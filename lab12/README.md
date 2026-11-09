# Lab 12 — Producer, Consumer and the Other Tools

**Platform:** Linux (openSUSE Tumbleweed, POSIX threads).

## Goal
Combine POSIX semaphores, mutexes, and condition variables to solve classic synchronization problems and eliminate a multi-mutex deadlock:
1. **Part 1:** Implement the bounded-buffer producer/consumer pattern using two counting semaphores (`empty`, `full`) and a mutex ([`prodcons.c`](prodcons.c)), and analyze the deadlock caused by locking the mutex before waiting on `empty`.
2. **Part 2:** Implement thread ordering with a state variable (`ready`), a mutex, and a condition variable ([`condvar.c`](condvar.c)), explaining why `pthread_cond_wait` requires a `while` loop.
3. **Part 3:** Implement the first readers-writers problem using `room` and `rlock` semaphores ([`readers.c`](readers.c)), and analyze reader concurrency, counter protection, and writer starvation.
4. **Part 4:** Reproduce a two-thread AB-BA deadlock, observe non-deterministic hangs without `sleep(1)`, and remove the deadlock by enforcing a consistent lock ordering ([`deadlock.c`](deadlock.c)).

## Files in this directory
- [Compiled report](../reports/lab12.pdf) · [LaTeX source](lab12.tex)
- [`report.md`](report.md) — complete observations and answers for Parts 1 to 4
- [`prodcons.c`](prodcons.c), [`condvar.c`](condvar.c), [`readers.c`](readers.c), [`deadlock.c`](deadlock.c) (fixed version) — C source files
- [`evidence/`](evidence/) — captured command outputs for all runs and experiments
