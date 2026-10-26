# Screenshot inventory

| Screenshot | Contents |
| --- | --- |
| [L2_01_setup.png](screenshots/L2_01_setup.png) | Compiler path and QEMU 10.2.1 visible; pause declaration at line 25. Git revision not captured here. |
| [L2_02_boot.png](screenshots/L2_02_boot.png) | Successful xv6 boot and shell prompt; no fresh compilation output in this capture. |
| [L2_03_programs.png](screenshots/L2_03_programs.png) | Directory listing and beginning of README visible; README output is only partially captured. |
| [L2_04_shell.png](screenshots/L2_04_shell.png) | Echo and pipeline work; README has 48 lines, 336 words, 2441 bytes. |
| [L2_05_source_ls.png](screenshots/L2_05_source_ls.png) | First 40 lines show headers, formatting helper, and opening a path. |
| [L2_06_source_cat.png](screenshots/L2_06_source_cat.png) | All 43 source lines visible; enough to identify all direct system calls. |
| [L2_07_syscall_declarations.png](screenshots/L2_07_syscall_declarations.png) | System calls and userspace library declarations readable; pause at line 25. |
| [L2_08_syscall_kernel.png](screenshots/L2_08_syscall_kernel.png) | sys_read at line 69 and sys_write at line 83, with function bodies. |
| [L2_09_sleep_code.png](screenshots/L2_09_sleep_code.png) | Complete sleep program calls pause and validates argument count; transcribed into user/sleep.c. |

Build registration and sleep execution are recorded in [makefile.txt](evidence/makefile.txt), [build.txt](evidence/build.txt), and [sleep-runtime.txt](evidence/sleep-runtime.txt).
