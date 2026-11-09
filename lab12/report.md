# Lab 12: Producer, Consumer and the Other Tools

## Goal (Scopul lucrării)

The goal of this laboratory is to combine POSIX synchronization primitives (semaphores, mutexes, and condition variables) to solve classic concurrent programming problems and analyze deadlock creation and prevention:
1. **Part 1 (Producer and consumer):** Coordinate a fast producer thread and a slow consumer thread sharing a 4-slot circular buffer using two counting semaphores (`empty` initialized to `4`, `full` initialized to `0`) and a mutex (`lock`) (`prodcons.c`).
2. **Part 2 (The condition variable):** Solve the writer-before-reader ordering problem using an explicit state variable (`ready`), a mutex (`lock`), and a condition variable (`changed`) with a `while (!ready)` wait loop (`condvar.c`).
3. **Part 3 (Readers and writers):** Allow multiple concurrent readers into a shared room while guaranteeing exclusive access for writers using two semaphores (`room` and `rlock`) and an active reader counter (`readers.c`).
4. **Part 4 (Build a deadlock, then remove it):** Reproduce a two-thread AB-BA deadlock (`timeout 5 ./deadlock` exiting with code `124`), test the timing sensitivity when `sleep(1)` is removed, and eliminate the deadlock permanently by enforcing a consistent lock acquisition order (`deadlock.c`).

---

## Part 1 — Producer and consumer

### Build and Execution (`prodcons.c`)

```console
$ gcc -Wall -pthread -o prodcons prodcons.c
$ ./prodcons
put  1  (in buffer: 1)
put  2  (in buffer: 2)
put  3  (in buffer: 3)
put  4  (in buffer: 4)
          take 1  (in buffer: 3)
put  5  (in buffer: 4)
          take 2  (in buffer: 3)
put  6  (in buffer: 4)
          take 3  (in buffer: 3)
put  7  (in buffer: 4)
          take 4  (in buffer: 3)
put  8  (in buffer: 4)
          take 5  (in buffer: 3)
          take 6  (in buffer: 2)
          take 7  (in buffer: 1)
          take 8  (in buffer: 0)
```

### Observe (Part 1)

1. **The producer is fast, yet `"in buffer"` never goes above 4. Which call stops it?**
   The call `sem_wait(&empty);` at the top of the loop in `producer()` stops it. Because `empty` starts at `SIZE` (`4`), after the producer inserts 4 items without the slow consumer taking any, `empty` reaches `0`. On the 5th iteration, `sem_wait(&empty)` blocks the producer until the consumer takes an item and calls `sem_post(&empty)`.

2. **Who waits at the start: the consumer or the producer? Which call makes it wait?**
   If the **consumer** runs first at the start (since `pthread_create(&c, ...)` is called before `pthread_create(&p, ...)`), the **consumer waits**, blocked by `sem_wait(&full);` because `full` is initialized to `0` (`sem_init(&full, 0, 0)`) and there are no items in the buffer yet until the producer calls `sem_post(&full)`.

3. **In `producer()`, swap the first two lines: lock the mutex BEFORE `sem_wait(&empty)`. Run it. What happens when the buffer is full, and why? (Ctrl-C to stop it, then put the lines back.)**
   Output before hanging:
   ```console
   put  1  (in buffer: 1)
   put  2  (in buffer: 2)
   put  3  (in buffer: 3)
             take 1  (in buffer: 2)
   put  4  (in buffer: 3)
   put  5  (in buffer: 4)
   exit code: 124
   ```
   When the buffer becomes full (`count == 4`, so `empty == 0`), **the program deadlocks (hangs forever)**:
   - The producer calls `pthread_mutex_lock(&lock)` first, acquiring `lock`, and then calls `sem_wait(&empty)`. Because `empty == 0`, the producer goes to sleep **while still holding `lock`**.
   - The consumer wakes up after `usleep(300000)`, passes `sem_wait(&full)`, and calls `pthread_mutex_lock(&lock)` to remove an item from the buffer. Because the sleeping producer holds `lock`, the consumer blocks waiting for `lock` and can never reach `sem_post(&empty)`.
   - Thus, the producer waits for the consumer to post `empty`, while the consumer waits for the producer to unlock `lock`—a classic deadlock caused by sleeping on a semaphore while holding a mutex.

---

## Part 2 — The condition variable

### Build and Execution (`condvar.c`)

```console
$ gcc -Wall -pthread -o condvar condvar.c
$ ./condvar
B: not ready, I sleep
A: message written
B: I read "hello"
```

### Observe (Part 2)

1. **In `order.c` (Lab 11) the semaphore itself remembered that the message was ready. Here, which variable remembers it?**
   The ordinary integer variable **`ready`** (`int ready = 0;`, set to `1` by `writer()`) remembers the state. Condition variables themselves are stateless: if `pthread_cond_signal(&changed)` is called when no thread is waiting, the signal is not stored, so threads must always inspect the state variable `ready` under `lock` before deciding whether to sleep.

2. **Replace `"while (!ready)"` with `"if (!ready)"`, compile, run. Does it still work here?**
   Output with `if (!ready)`:
   ```console
   B: not ready, I sleep
   A: message written
   B: I read "hello"
   ```
   **Yes**, it still works in this simple two-thread program because there is only a single reader thread waiting on `changed`, `writer()` sets `ready = 1` before signaling, and no spurious wakeup occurred during the 1-second sleep.

3. **Then why do we write `while`? Give one reason why a thread can wake up and still find `ready == 0`.**
   We must always write `while (!ready)` because a waiting thread can return from `pthread_cond_wait` and still find `ready == 0` for two standard reasons:
   - **Spurious wakeups:** The POSIX specification (`pthread_cond_wait`) and OS kernel (`futex`) explicitly permit a waiting thread to wake up occasionally without any thread having called `pthread_cond_signal` (for instance, due to signal delivery or kernel scheduling internals).
   - **Stolen state / multiple waiters (Mesa semantics):** When multiple reader/consumer threads are waiting (or a third thread acquires `lock` between `pthread_cond_signal` and the woken thread reacquiring `lock`), another thread can run first, consume the condition (resetting `ready = 0`), and release `lock` before the woken thread resumes. Re-checking in a `while` loop guarantees the invariant `ready != 0` holds when leaving the loop.

---

## Part 3 — Readers and writers

### Build and Execution (`readers.c`)

```console
$ gcc -Wall -pthread -o readers readers.c
$ ./readers
writer 1 writes 10 (alone)
reader 1 reads 10  (readers inside: 1)
reader 2 reads 10  (readers inside: 2)
reader 3 reads 10  (readers inside: 3)
writer 2 writes 20 (alone)
```

### Observe (Part 3)

1. **Three readers are inside at the same time, but a writer is always alone. Which two lines of `reader()` make that possible?**
   The two lines in the entry section of `reader()`:
   ```c
   if (++readers == 1)
       sem_wait(&room);
   ```
   (paired on exit with `if (--readers == 0) sem_post(&room);`). Only the **first** reader (`++readers == 1`) waits for and locks `room`, blocking writers. Subsequent readers (`readers == 2, 3, ...`) see `++readers != 1`, skip `sem_wait(&room)`, and enter the room concurrently alongside the first reader. The room remains locked against writers until the **last** reader leaves (`--readers == 0`) and calls `sem_post(&room)`.

2. **Why does the counter `readers` need its own lock (`rlock`)? What could go wrong without it?**
   `readers` is a shared variable modified by all reader threads (`++readers` and `--readers`), and its updated value determines whether a thread calls `sem_wait(&room)` or `sem_post(&room)`. Without `rlock`:
   - **Data race on `++readers` / `--readers`:** Two readers arriving simultaneously could both read `readers == 0` and both think they are the first reader (`++readers == 1`), causing both to call `sem_wait(&room)`. The first would lock `room`, and the second would block on `room` while readers are supposed to share the room; or one increment could be lost so `readers` is `1` when two readers are inside, causing the first exiting reader (`--readers == 0`) to call `sem_post(&room)` and let a writer in **while the second reader is still reading**.
   - Conversely, if a decrement is lost, `readers` may never return to `0`, so `sem_post(&room)` is never called and all future writers deadlock forever.

3. **Imagine readers that keep arriving without a pause. What happens to `writer 2`? (This problem has a name: starvation.)**
   If new readers keep arriving continuously before the currently active readers finish, the counter `readers` **never drops to `0`**. Because `sem_post(&room)` is only called when `--readers == 0`, the `room` semaphore is never released, and `writer 2` waits in `sem_wait(&room)` forever—a classic **writer starvation** problem (caused by reader-preference synchronization).

---

## Part 4 — Build a deadlock, then remove it

### Broken Version Execution (`exit code: 124`)

```console
$ gcc -Wall -pthread -o deadlock deadlock.c
$ timeout 5 ./deadlock; echo "exit code: $?"
one: I have A, I want B
two: I have B, I want A
exit code: 124
```

### Fixed Version Execution (`deadlock.c`, locking `A` then `B` in both threads)

```console
$ gcc -Wall -pthread -o deadlock deadlock.c
$ timeout 5 ./deadlock; echo "exit code: $?"
one: I have A, I want B
one: I have both
two: I have B, I want A
two: I have both
done
exit code: 0
```

### Observe (Part 4)

1. **In the stuck run, which thread holds which mutex, and which one is each thread waiting for? Draw the circle.**
   - Thread `one` **holds mutex `A`** and is **waiting for mutex `B`** (`pthread_mutex_lock(&B)`).
   - Thread `two` **holds mutex `B`** and is **waiting for mutex `A`** (`pthread_mutex_lock(&A)`).

   Wait-for cycle:
   ```text
        holds                  wants
   [A] -------> (Thread one) --------> [B]
    ^                                   |
    |   wants                  holds    v
    +---------- (Thread two) <----------+
   ```
   Or concisely between threads: `Thread one --(waits for B held by)--> Thread two --(waits for A held by)--> Thread one`.

2. **Remove both `sleep(1)` lines from the ORIGINAL version and run it ten times. Does it still always hang? Why is that worse, not better?**
   Running the original opposite-order version without `sleep(1)` ten times produced **9 hangs (`exit code: 124`) and 1 successful completion (`exit code: 0` on run 10)**:
   ```console
   === run 9 ===
   one: I have A, I want B
   two: I have B, I want A
   exit code: 124
   === run 10 ===
   one: I have A, I want B
   one: I have both
   two: I have B, I want A
   two: I have both
   done
   exit code: 0
   ```
   **No, it does not always hang**—if thread `one` is scheduled and finishes locking `A`, locking `B`, and unlocking both before thread `two` starts and locks `B`, the program completes normally.
   **Why this is worse, not better:** Removing `sleep(1)` does not fix the bug; it hides the bug behind a narrow timing window, turning a deterministic, easily reproducible deadlock into a **non-deterministic race / Heisenbug** that may pass tests 99% of the time and then freeze unpredictably in production under load.

3. **In one sentence: why can taking the locks in the same order never deadlock?**
   When every thread acquires locks in the same global order (`A` before `B`), no circular wait can ever form because a thread waiting for `B` already holds `A`, so no other thread can hold `B` while waiting for `A`.
