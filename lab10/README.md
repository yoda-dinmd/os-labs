# Lab 10 — Add Your Own System Call: `trace`

**Platform: xv6 on RISC-V.** The handout explicitly targets the `xv6-riscv` tree, uses syscall number 23 because number 22 is already `sync`, and requires the RISC-V toolchain. The modified working tree is `/home/victor/xv6-riscv` on the development machine.

- [Compiled report](../reports/lab10.pdf) · [LaTeX source](lab10.tex)
- [`xv6-trace.patch`](xv6-trace.patch) contains the kernel and syscall integration changes, the new `user/trace.c`, and the `args` and `sleep` user programs referenced by the xv6 Makefile.
- [`notes.txt`](notes.txt) contains the Part 3 answers.
- `part1.png` records the usage check and the expected unknown syscall before adding the kernel handler; `part2.png` records the working read trace.

The patch captures the Lab 10 changes in the existing working tree, including the local RISC-V toolchain prefix adjustment. It also includes `args` and `sleep`, which are referenced in the modified Makefile and were used in earlier xv6 work.
