# Lab 9 — Watching System Calls with `strace`

**Platform:** Linux (openSUSE Tumbleweed).

## Goal
Observe the system-call boundary between user-space processes and the Linux kernel using `strace`:
1. **Part 1:** Trace `ls -l` (`strace -f -e trace=openat,read,write,close ls -l`) and identify how it reads directory metadata and writes to standard output.
2. **Part 2:** Write, compile, and trace [`syscalls.c`](syscalls.c), a C program that invokes `open`, `write`, `read`, and `close` directly, and explain file descriptor allocation starting at descriptor `3`.
3. **Part 3:** Count a single kind of system call using `strace -c` (`strace -c ls -l`) and single-call filtering (`strace -e trace=openat ls -l 2>&1 >/dev/null | grep -c '^openat'`).

## Files in this directory
- [Compiled report](../reports/lab9.pdf) · [LaTeX source](lab9.tex)
- [`report.md`](report.md) and [`notes.md`](notes.md) — complete observations and answers for Parts 1, 2, and 3
- [`syscalls.c`](syscalls.c) — direct system-call C program (Part 2)
- [`evidence/`](evidence/) — captured full trace outputs and single-call counts (`part1-ls-trace.txt`, `part2-syscalls-trace.txt`, `part3-strace-c.txt`, `part3-strace-c-filtered.txt`, `part3-*-count.txt`)
- `pic1.png`, `pic2.png`, `pic3.png` — terminal screenshots for Parts 1, 2, and 3
