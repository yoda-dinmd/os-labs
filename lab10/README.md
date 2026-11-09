# Lab 10 — Add Your Own System Call: `trace`

**Platform:** xv6 on RISC-V (`xv6-riscv`).

## Goal
Add a new system call, `trace` (syscall number 23), and user program `user/trace.c` to the xv6 RISC-V kernel so that a process and its forked descendants print every system call matching a bit mask (`trace mask command [args...]`).

## Files in this directory
- [Compiled report](../reports/lab10.pdf) · [LaTeX source](lab10.tex)
- [`xv6-trace.patch`](xv6-trace.patch) — complete git diff against `xv6-riscv` implementing `SYS_trace` (`23`), `user/trace.c`, `sys_trace`, `syscallnames[]`, `trace_mask` inheritance in `fork()`, and `Makefile` integration
- [`notes.txt`](notes.txt) — Part 3 command outputs (`trace 2147483647 grep hello README`, `trace 2 usertests forkforkfork`, `trace 65536 echo hi`) and answers to the Observe questions
- `part1.png`, `part2.png`, `part3.png` — screenshots of Check 1 (unknown syscall 23 before kernel handler), Check 2 (`trace 32 grep hello README`), and Part 3 (`trace 2147483647 grep hello README`, `trace 2 usertests forkforkfork`, `trace 65536 echo hi`)
