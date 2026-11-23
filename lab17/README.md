# Lab 17 — Threads Share Memory, and Race

**Platform:** Linux, POSIX threads.

The program measures lost updates when two or four workers increment the same shared counter one million times each.

```bash
cd lab17
gcc -Wall -Wextra -pthread -o fire fire.c
./fire       # Part 1: two workers
./fire 4     # Part 2: four workers
```

Use the compiler command as shown. The experiment deliberately races on an ordinary C variable and the results vary by run.

- [Source](fire.c) · [Observations](report.md)
- [Two-worker runs](evidence/two-threads.txt) · [Four-worker runs](evidence/four-threads.txt)
- [Compiled report](../reports/lab17.pdf) · [LaTeX source](lab17.tex)
