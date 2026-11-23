# Lab 18 — Build a User-Level Thread Switch

**Platform:** xv6 on RISC-V.

`uthread` starts three cooperative user threads, each with a saved register context and an 8 KiB stack. The round-robin scheduler switches at explicit yields and discards finished threads.

The patch contains both user sources and the xv6 Makefile changes. To install it in another checkout of the course xv6 tree, run these commands from the root of this repository.

```bash
git -C ~/xv6-riscv apply --check "$PWD/lab18/xv6-uthread.patch"
git -C ~/xv6-riscv apply "$PWD/lab18/xv6-uthread.patch"
cd ~/xv6-riscv
make qemu
```

If your course toolchain uses the openSUSE prefix and Make does not detect it, use `make qemu TOOLPREFIX=riscv64-suse-linux-`.

Run `uthread` at the xv6 shell. After the three startup messages, the counters alternate in the order `c`, `a`, `b` from 0 to 99. Each thread reports 100 iterations. The scheduler prints `thread_schedule: no runnable threads` and returns control to the shell. To leave QEMU, press Ctrl+A followed by X.

- [xv6 patch](xv6-uthread.patch) · [C source](user/uthread.c) · [Assembly source](user/uthread_switch.S)
- [Observe answer](notes.txt) · [QEMU transcript](evidence/uthread.txt)
- [Compiled report](../reports/lab18.pdf) · [LaTeX source](lab18.tex)
