# Screenshot review

All 23 currently supplied PNG files were inspected. Originals are preserved unchanged in `screenshots/`.

| Screenshot | Findings |
| --- | --- |
| [L1_01_environment.png](screenshots/L1_01_environment.png) | Username, hostname, kernel, uptime visible; left edge slightly cropped but key values readable. |
| [L1_02_directories.png](screenshots/L1_02_directories.png) | Working directory, home, and /etc shown. Uses ls -la without /; pwd and root listing not captured. |
| [L1_03_files.png](screenshots/L1_03_files.png) | Creation, copy, rename, contents, and deletion shown; owner/group victor; file size 24 bytes. |
| [L1_04_permissions.png](screenshots/L1_04_permissions.png) | Modes 664 initially, 600 after restriction, then 644 shown. |
| [L1_05_processes.png](screenshots/L1_05_processes.png) | 222 processes excluding header; PID 1 identified as systemd. |
| [L1_06_top.png](screenshots/L1_06_top.png) | 251 tasks, one running and 250 sleeping. Later snapshot than the process count. |
| [L1_07_background.png](screenshots/L1_07_background.png) | PID 9453 starts, is listed, and terminates; final process lookup empty. |
| [L1_08_proc.png](screenshots/L1_08_proc.png) | PID 9765 shows S (sleeping), /proc entries, and termination. |
| [L1_09_memory.png](screenshots/L1_09_memory.png) | 3.3 GiB total, 272 MiB free, 962 MiB available; no swap; VmRSS 7752 kB. |
| [L1_10_storage.png](screenshots/L1_10_storage.png) | Root /dev/sda2, ext4; virtual disk 50 GiB; work directory 12 KiB. |
| [L1_11_devices.png](screenshots/L1_11_devices.png) | Device listing, /dev/null character device, cdrom symlink, and mounts visible. |
| [L2_01_setup.png](screenshots/L2_01_setup.png) | Compiler path and QEMU 10.2.1 visible; pause declaration at line 25. Git revision not captured here. |
| [L2_02_boot.png](screenshots/L2_02_boot.png) | Successful xv6 boot and shell prompt; no fresh compilation output in this capture. |
| [L2_03_programs.png](screenshots/L2_03_programs.png) | Directory listing and beginning of README visible; README output is only partially captured. |
| [L2_04_shell.png](screenshots/L2_04_shell.png) | Echo and pipeline work; README has 48 lines, 336 words, 2441 bytes. |
| [L2_05_source_ls.png](screenshots/L2_05_source_ls.png) | First 40 lines show headers, formatting helper, and opening a path. |
| [L2_06_source_cat.png](screenshots/L2_06_source_cat.png) | All 43 source lines visible; enough to identify all direct system calls. |
| [L2_07_syscall_declarations.png](screenshots/L2_07_syscall_declarations.png) | System calls and userspace library declarations readable; pause at line 25. |
| [L2_08_syscall_kernel.png](screenshots/L2_08_syscall_kernel.png) | sys_read at line 69 and sys_write at line 83, with function bodies. |
| [L2_09_sleep_code.png](screenshots/L2_09_sleep_code.png) | Complete sleep program calls pause and validates argument count; transcribed into user/sleep.c. |
| [L2_10_makefile.png](screenshots/L2_10_makefile.png) | Sleep entry present, but blank separator before fs.img removed. Needs correction and a new capture. |
| [L2_11_sleep_test.png](screenshots/L2_11_sleep_test.png) | sleep 10 returns without an error; sleep prints the usage message. The earlier sleep 10 20 attempt reports exec sleep failed; the successful repeat is shown in fixed2.png. |
| [fixed2.png](screenshots/fixed2.png) | sleep 10 20 prints usage: sleep <ticks> and returns to the prompt, confirming the extra-argument check. |

## Additional evidence

- **Makefile evidence:** the existing capture predates restoration of the blank separator before `fs.img:`; a corrected capture would document the final layout.
- **Useful for completeness:** `pwd` and `ls -la /` in Ubuntu; these are absent from the directory capture.
- **Reproducibility:** capture `git rev-parse --short HEAD` in the VM's xv6 checkout.
- The README screenshot is an excerpt, but the separate `wc` result records its full size.

The runtime screenshots confirm execution and both argument-count usage checks. Pause duration and exit status are not measured by the screenshot.
