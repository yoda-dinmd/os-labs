# Lab 11: The Race and the Semaphore

## Goal (Scopul lucrării)

The goal of this laboratory is to observe a data race when two threads concurrently increment a shared variable without synchronization, eliminate the race using both a POSIX mutex and a POSIX semaphore, and explore the three classic roles of a semaphore in concurrent programming:
1. **Part 1 (See the race):** Run two threads that each increment a shared `counter` variable `1,000,000` times without protection (`race.c`) and explain why updates are lost at the CPU instruction level.
2. **Part 2 (Fix it: mutex, then semaphore):** Protect the critical section around `counter++` first with a `pthread_mutex_t` (`mutex.c`) and then with a binary `sem_t` initialized to `1` (`sem.c`), verify that both always reach `2,000,000`, test `sem_init(&s, 0, 2)`, and compare execution times.
3. **Part 3 (A semaphore as a signal):** Enforce ordering between a writer thread `A` and a reader thread `B` using a semaphore initialized to `0` (`order.c`).
4. **Part 4 (A semaphore as a counter: the parking lot):** Limit concurrent access to a shared resource of capacity `3` among `6` threads using a counting semaphore initialized to `3` (`parking.c`).
5. **Optional (No lock at all):** Replace the lock with a C11 atomic fetch-and-add instruction (`atomic.c`) and compare its performance with the mutex and semaphore versions.

---

## Part 1 — See the race

### Build and Execution (`race.c`)

```console
$ gcc -Wall -pthread -o race race.c
$ ./race
counter = 1031820 (expected 2000000)
$ ./race
counter = 1321476 (expected 2000000)
$ ./race
counter = 1153608 (expected 2000000)
```

### Observe (Part 1)

1. **`counter++` looks like one step. Write the three steps the CPU really does.**
   At the machine level (without optimization keeping `counter` in a register), incrementing a variable in memory requires three distinct steps:
   1. **Load (Read):** Read the current value of `counter` from RAM/cache into a CPU register (e.g., `movq counter(%rip), %rax`).
   2. **Add (Modify):** Add `1` to the value inside the CPU register (e.g., `addq $1, %rax`).
   3. **Store (Write):** Write the updated register value back to `counter` in memory (e.g., `movq %rax, counter(%rip)`).

2. **Describe, with those steps, how two threads can lose one addition.**
   Suppose `counter` in memory currently holds `100`:
   - Thread A executes **Step 1 (Load)** and reads `100` into its own register.
   - Before Thread A writes back (or simultaneously on another CPU core), Thread B executes **Step 1 (Load)** and also reads `100` into its register.
   - Thread A executes **Step 2 (Add)** (`100 + 1 = 101`) and **Step 3 (Store)**, writing `101` to `counter`.
   - Thread B executes **Step 2 (Add)** (`100 + 1 = 101`) and **Step 3 (Store)**, overwriting `counter` with `101`.
   Although both threads performed an increment, `counter` increased from `100` to `101` instead of `102`—one addition was lost.

3. **Why is the result different at every run?**
   The OS kernel scheduler preempts threads at non-deterministic points depending on timer interrupts, CPU core assignment, cache coherence traffic, and background system load. Because the exact interleaving of the two threads' load-add-store sequences varies from run to run, the number of colliding increments (lost updates) changes on every execution.

---

## Part 2 — Fix it: mutex, then semaphore

### Build and Execution (`mutex.c` and `sem.c`)

```console
$ gcc -Wall -pthread -o mutex mutex.c
$ ./mutex
counter = 2000000 (expected 2000000)
$ ./mutex
counter = 2000000 (expected 2000000)
$ ./mutex
counter = 2000000 (expected 2000000)

$ gcc -Wall -pthread -o sem sem.c
$ ./sem
counter = 2000000 (expected 2000000)
$ ./sem
counter = 2000000 (expected 2000000)
$ ./sem
counter = 2000000 (expected 2000000)
```

### Timing Comparison (`2.3 The price of the lock`)

```console
$ time ./race
counter = 1125516 (expected 2000000)
real 0.015s  user 0.025s  sys 0.001s

$ time ./mutex
counter = 2000000 (expected 2000000)
real 0.157s  user 0.195s  sys 0.104s

$ time ./sem
counter = 2000000 (expected 2000000)
real 0.454s  user 0.557s  sys 0.344s
```

### Observe (Part 2)

1. **Run `mutex` and `sem` three times each. Is the answer always `2 000 000`?**
   **Yes.** Both `./mutex` and `./sem` print `counter = 2000000 (expected 2000000)` on every run because only one thread can enter the critical section (`counter++`) at a time, serializing the load-add-store sequence.

2. **What would happen in `sem.c` with `sem_init(&s, 0, 2)`? Try it and explain.**
   Running `sem.c` with `sem_init(&s, 0, 2)` three times produced:
   ```console
   counter = 1983089 (expected 2000000)
   counter = 1989142 (expected 2000000)
   counter = 1988601 (expected 2000000)
   ```
   The result is wrong again (below `2,000,000`). Initializing the semaphore to `2` allows **up to two threads** to succeed in `sem_wait(&s)` simultaneously (`2 -> 1` for the first thread, `1 -> 0` for the second thread). Since there are only two threads in total, both can enter the critical section at the same time, reintroducing the data race on `counter++`.

3. **Why is the locked version slower?**
   The locked versions (`mutex` at `0.157s` and `sem` at `0.454s`, vs. `0.015s` for `race`) are slower for three reasons:
   - **Atomic instruction overhead:** Every lock/unlock (`2,000,000` pairs in total) executes hardware atomic read-modify-write instructions (such as `lock cmpxchg`) and memory barriers that flush pipelines and bounce cache lines between CPU cores.
   - **Kernel blocking (`futex` system calls):** Whenever one thread tries to acquire the lock while the other holds it, the contending thread blocks and wakes via Linux `futex` system calls (reflected in the jump in `sys` time from `0.001s` to `0.104s` / `0.344s`), incurring context switches.
   - **Serialization:** Only one thread can execute the critical section at a time, eliminating parallel progress on the counter.

---

## Part 3 — A semaphore as a signal

### Build and Execution (`order.c`)

```console
$ gcc -Wall -pthread -o order order.c
$ ./order
B: waiting for the message...
A: message written
B: I read "hello"
```

### Observe (Part 3)

1. **Change the start value to 1 (`sem_init(&ready, 0, 1)`), compile, run. What does B read now, and why?**
   Output with `sem_init(&ready, 0, 1)`:
   ```console
   B: waiting for the message...
   B: I read ""
   A: message written
   ```
   Thread B reads the empty string `""` before Thread A writes `"hello"`. Because `ready` starts at `1`, Thread B's call to `sem_wait(&ready)` immediately decrements `ready` from `1` to `0` without blocking, so B prints the global `message` array (which is zero-initialized at startup) while Thread A is still inside `sleep(1)`.

2. **Put it back to 0. In one sentence: what does a semaphore that starts at 0 mean?**
   A semaphore that starts at `0` represents an **event or signal that has not happened yet**, forcing any thread calling `sem_wait` to block until another thread signals completion with `sem_post`.

---

## Part 4 — A semaphore as a counter: the parking lot

### Build and Execution (`parking.c`)

```console
$ gcc -Wall -pthread -o parking parking.c
$ ./parking
car 1 enters (inside: 1)
car 2 enters (inside: 2)
car 3 enters (inside: 3)
car 1 leaves (inside: 2)
car 3 leaves (inside: 1)
car 2 leaves (inside: 0)
car 4 enters (inside: 1)
car 5 enters (inside: 2)
car 6 enters (inside: 3)
car 4 leaves (inside: 2)
car 5 leaves (inside: 1)
car 6 leaves (inside: 0)
```

### Observe (Part 4)

1. **What is the largest value of `"inside"` you see? Which line of code guarantees it?**
   The largest value of `inside` is **`3`**. It is guaranteed by `sem_wait(&places);` (together with `sem_init(&places, 0, PLACES);` where `PLACES` is `3`), which decrements the semaphore on each entry and blocks the 4th, 5th, and 6th car threads when the counter reaches `0` until a parked car calls `sem_post(&places);`.

2. **Set `PLACES` to 1. What does the semaphore become?**
   Output with `#define PLACES 1`:
   ```console
   car 1 enters (inside: 1)
   car 1 leaves (inside: 0)
   car 2 enters (inside: 1)
   car 2 leaves (inside: 0)
   car 3 enters (inside: 1)
   car 3 leaves (inside: 0)
   car 4 enters (inside: 1)
   car 4 leaves (inside: 0)
   car 5 enters (inside: 1)
   car 5 leaves (inside: 0)
   car 6 enters (inside: 1)
   car 6 leaves (inside: 0)
   ```
   When `PLACES` is set to `1`, the counting semaphore becomes a **binary semaphore (a mutex / lock)** that admits only one car at a time (`inside` never exceeds `1`).

---

## Optional — No lock at all (`atomic.c`)

```console
$ gcc -Wall -pthread -o atomic atomic.c
$ time ./atomic
counter = 2000000 (expected 2000000)
real 0.080s  user 0.154s  sys 0.001s
```

Using `atomic_fetch_add(&counter, 1)` performs the load, increment, and store as a single indivisible CPU instruction (`lock xaddq`), producing the exact result `2000000` in `0.080s` (`sys 0.001s`, zero kernel `futex` transitions)—twice as fast as the mutex (`0.157s`) and nearly six times as fast as the semaphore (`0.454s`).
