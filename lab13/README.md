# Lab 13 — Semaphores in Your Own Kernel

**Platform:** xv6 on RISC-V (`xv6-riscv`).

## Goal
Implement counting semaphores inside the xv6 RISC-V kernel as new system calls (`sem_init` = 24, `sem_down` = 25, `sem_up` = 26, plus optional `sem_trydown` = 27) using a kernel table of 8 semaphores (`kernel/sem.c`), per-semaphore spinlocks, and two-step kernel sleeping (`sleep_prepare(s)` + `sleep()` and `wakeup(s)`).

## Files in this directory
- [Compiled report](../reports/lab13.pdf) · [LaTeX source](lab13.tex)
- [`notes.txt`](notes.txt) — Check 1, Check 2, Part 3 (`mix` and `mix lock`), Optional (`trydowntest`) outputs and answers to the four Observe questions
- [`xv6-sem.patch`](xv6-sem.patch) — complete git diff against `xv6-riscv` implementing `SYS_sem_init` (24), `SYS_sem_down` (25), `SYS_sem_up` (26), `SYS_sem_trydown` (27), `kernel/sem.c`, `user/semtest.c`, `user/mix.c`, and `user/trydowntest.c`
- [`kernel/sem.c`](kernel/sem.c) — kernel semaphore table and system call handlers
- [`user/semtest.c`](user/semtest.c), [`user/mix.c`](user/mix.c), [`user/trydowntest.c`](user/trydowntest.c) — xv6 user test programs
- [`evidence/`](evidence/) — captured QEMU build and execution transcripts
