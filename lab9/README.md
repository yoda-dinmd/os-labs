# Lab 9 — Watching System Calls with `strace`

**Platform:** Linux (openSUSE Tumbleweed). The report and source answer the handout's observations.

- [Compiled report](../reports/lab9.pdf) · [LaTeX source](lab9.tex)
- [`syscalls.c`](syscalls.c) and the captured executable output in `pic2.png`
- `pic1.png` shows the end of the filtered `ls -l` trace and the measured pipeline count.
- [`notes.md`](notes.md) contains the experiment notes and answers.

The `strace` command filters `openat`, `read`, `write` and `close`; `wc -l` counted 187 displayed calls in that run. Some `ENOENT` results are failed probes for optional locale catalogs. `ls -l` lists one directory and does not recurse.
