# Lab 11 — The Race and the Semaphore

**Platform:** Linux (openSUSE Tumbleweed, POSIX threads).

## Goal
Observe a multi-threaded data race on a shared counter and fix it with POSIX synchronization primitives, demonstrating a semaphore in three roles:
1. **Part 1:** Observe lost updates when two threads increment a shared variable without synchronization ([`race.c`](race.c)).
2. **Part 2:** Fix the race with a POSIX mutex ([`mutex.c`](mutex.c)) and a binary semaphore ([`sem.c`](sem.c)), test `sem_init(&s, 0, 2)`, and compare execution times.
3. **Part 3:** Use a semaphore initialized to `0` as an ordering signal between a writer and a reader thread ([`order.c`](order.c)).
4. **Part 4:** Use a counting semaphore initialized to `3` to bound concurrent access to a 3-slot parking lot across 6 threads ([`parking.c`](parking.c)).
5. **Optional:** Use lock-free C11 atomics ([`atomic.c`](atomic.c)) to increment the counter in a single indivisible CPU instruction.

## Files in this directory
- [Compiled report](../reports/lab11.pdf) · [LaTeX source](lab11.tex)
- [`report.md`](report.md) — complete observations and answers for Parts 1 to 4 (and Optional)
- [`race.c`](race.c), [`mutex.c`](mutex.c), [`sem.c`](sem.c), [`order.c`](order.c), [`parking.c`](parking.c), [`atomic.c`](atomic.c) — C source files
- [`evidence/`](evidence/) — captured command outputs and timing runs
