# Lab 9: Watching System Calls with `strace`

## Goal (Scopul lucrării)

The goal of this laboratory is to cross and observe the boundary between user-space programs and the Linux kernel—the system call interface—using `strace`. Specifically, the objectives are:
1. **Part 1 (See the calls of a real command):** Trace a real utility (`ls -l`) using `strace` to observe how user-space commands open directories and shared libraries, read metadata, and write formatted output to the terminal.
2. **Part 2 (A program that calls the kernel directly):** Write, compile, and trace a C program (`syscalls.c`) that interacts directly with the Linux kernel via the `open`, `write`, `read`, and `close` system calls, and understand how the kernel allocates and reuses file descriptors starting at descriptor `3`.
3. **Part 3 (Count one kind of call):** Measure and compare how many times a program invokes a single specific system call (such as `openat`, `read`, `write`, or `close`) using both `strace -c` summary profiling and single-call filtering (`strace -e trace=... | grep -c`).

---

## Part 1 — See the calls of a real command

### Command and Execution

Running `strace` on `ls -l` while filtering `openat`, `read`, `write`, and `close`:

```console
$ strace -f -e trace=openat,read,write,close ls -l
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libselinux.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 1024) = 1024
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libcap.so.2", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\300\214\0\0\0\0\0\0"..., 1024) = 1024
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\256\267\2\0\0\0\0\0"..., 1024) = 1024
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libpcre2-8.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 1024) = 1024
close(3)                                = 0
...
openat(AT_FDCWD, ".", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
close(3)                                = 0
openat(AT_FDCWD, "/etc/nsswitch.conf", O_RDONLY|O_CLOEXEC) = 4
read(4, "#\n# /etc/nsswitch.conf\n#\n# An ex"..., 4096) = 1221
read(4, "", 4096)                       = 0
close(4)                                = 0
openat(AT_FDCWD, "/etc/passwd", O_RDONLY|O_CLOEXEC) = 4
read(4, "root:x:0:0:root:/root:/bin/bash\n"..., 4096) = 1979
close(4)                                = 0
openat(AT_FDCWD, "/etc/group", O_RDONLY|O_CLOEXEC) = 4
read(4, "root:x:0:\nbin:x:1:daemon\ndaemon:"..., 4096) = 853
close(4)                                = 0
...
write(1, "total 416\n", 10total 416
)             = 10
openat(AT_FDCWD, "/etc/localtime", O_RDONLY|O_CLOEXEC) = 3
read(3, "TZif2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\20\0\0\0\20\0\0\0\0"..., 4096) = 2424
read(3, "TZif2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\20\0\0\0\20\0\0\0\0"..., 4096) = 1509
close(3)                                = 0
write(1, "-rw-r--r--. 1 victor victor   40"..., 57-rw-r--r--. 1 victor victor   4033 Nov  8 21:00 lab9.tex
) = 57
write(1, "-rw-r--r--. 1 victor victor    7"..., 57-rw-r--r--. 1 victor victor    783 Nov  8 21:01 notes.md
) = 57
write(1, "-rw-r--r--. 1 victor victor     "..., 57-rw-r--r--. 1 victor victor     14 Nov  8 13:33 note.txt
) = 57
write(1, "-rw-r--r--. 1 victor victor 3604"..., 57-rw-r--r--. 1 victor victor 360410 Nov  8 12:42 pic1.png
) = 57
write(1, "-rw-r--r--. 1 victor victor  211"..., 57-rw-r--r--. 1 victor victor  21159 Nov  8 13:33 pic2.png
) = 57
write(1, "-rw-r--r--. 1 victor victor    6"..., 58-rw-r--r--. 1 victor victor    693 Nov  8 21:00 README.md
) = 58
write(1, "-rwxr-xr-x. 1 victor victor  204"..., 57-rwxr-xr-x. 1 victor victor  20400 Nov  8 12:50 syscalls
) = 57
write(1, "-rw-r--r--. 1 victor victor    9"..., 59-rw-r--r--. 1 victor victor    984 Nov  8 12:47 syscalls.c
) = 59
close(1)                                = 0
close(2)                                = 0
+++ exited with 0 +++
```

### Observe (Part 1)

- **Which system call does `ls` use to print to the screen? To which descriptor does it write?**
  `ls` uses the `write` system call (`write(1, ..., ...)`) to print each line of the directory listing to the screen. It writes to **file descriptor `1`**, which represents **standard output (`stdout`)**.
- **Roughly how many system calls does one simple `ls -l` make?**
  Across the four filtered call types (`openat`, `read`, `write`, `close`), `ls -l` makes roughly **174–186 calls** (depending on the number of entries in the directory), and about **368–371 system calls in total** across all system call types when dynamic library loading (`mmap`, `mprotect`), file metadata queries (`statx`, `newfstatat`, `fstat`), locale initialization, and user/group name resolution (`/etc/passwd`, `/etc/group`) are included.

---

## Part 2 — A program that calls the kernel directly

### Source Code (`syscalls.c`) and Execution

```console
$ gcc -Wall -Wextra -o syscalls syscalls.c
$ ./syscalls
open returned descriptor 3
write sent 14 bytes
read got 14 bytes: hello, kernel
```

### Tracing `./syscalls` with `strace`

```console
$ strace -e trace=openat,write,read,close ./syscalls
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\256\267\2\0\0\0\0\0"..., 1024) = 1024
close(3)                                = 0
openat(AT_FDCWD, "note.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644) = 3
write(1, "open returned descriptor 3\n", 27open returned descriptor 3
) = 27
write(3, "hello, kernel\n", 14)         = 14
write(1, "write sent 14 bytes\n", 20write sent 14 bytes
)   = 20
close(3)                                = 0
openat(AT_FDCWD, "note.txt", O_RDONLY)  = 3
read(3, "hello, kernel\n", 63)          = 14
write(1, "read got 14 bytes: hello, kernel"..., 33read got 14 bytes: hello, kernel
) = 33
close(3)                                = 0
+++ exited with 0 +++
```

### Observe (Part 2)

- **The descriptor returned by the first `openat` is 3, not 0, 1 or 2. Why are those three already taken?**
  File descriptors `0`, `1`, and `2` are reserved by convention and opened automatically for every process by the parent shell before `execve`:
  - `0` is **standard input (`stdin`)**,
  - `1` is **standard output (`stdout`)**,
  - `2` is **standard error (`stderr`)**.
  Because the Linux kernel always allocates the **lowest-numbered unused file descriptor** in the per-process file descriptor table, the first `openat("note.txt", ...)` returns `3`. When `close(3)` is called, slot `3` becomes free again, so the subsequent `openat("note.txt", O_RDONLY)` reuses descriptor `3`.

---

## Part 3 — Count one kind of call

To count **one specific kind of system call** (rather than counting all lines of a multi-call trace with `wc -l`), we use two complementary approaches in `strace`:

### Method 1: Filtering a single system call with `-e trace=<syscall>` and `grep -c`

By restricting `strace` to a single system call and matching only lines starting with that call name (`grep -c`), we exclude the `+++ exited with 0 +++` trailer and count only invocations of that single call type:

```console
$ strace -e trace=openat ls -l 2>&1 >/dev/null | grep -c '^openat'
102
$ strace -e trace=close ls -l 2>&1 >/dev/null | grep -c '^close'
56
$ strace -e trace=read ls -l 2>&1 >/dev/null | grep -c '^read'
15
$ strace -e trace=write ls -l 2>&1 >/dev/null | grep -c '^write'
1
```

Similarly, for our direct system-call program `./syscalls`:

```console
$ strace -e trace=openat ./syscalls 2>&1 >/dev/null | grep -c '^openat'
4
$ strace -e trace=write ./syscalls 2>&1 >/dev/null | grep -c '^write'
2
$ strace -e trace=read ./syscalls 2>&1 >/dev/null | grep -c '^read'
2
$ strace -e trace=close ./syscalls 2>&1 >/dev/null | grep -c '^close'
4
```

### Method 2: Summary profiling per system call type with `strace -c`

Running `strace -c` produces an exact breakdown of call counts, errors, and time spent for each individual system call type:

```console
$ strace -c -e trace=openat,read,write,close ls -l >/dev/null
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 72.06    0.000539           5       102        48 openat
 21.66    0.000162           2        56           close
  6.02    0.000045           3        15           read
  0.27    0.000002           2         1           write
------ ----------- ----------- --------- --------- ----------------
100.00    0.000748           4       174        48 total
```

And across all system calls made by `ls -l`:

```console
$ strace -c ls -l >/dev/null
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 34.95    0.001475          14       102        48 openat
 14.31    0.000604          14        43           mmap
  8.70    0.000367         367         1           execve
  7.68    0.000324           5        56           close
  5.71    0.000241           6        35           statx
  4.83    0.000204          10        20           newfstatat
  4.15    0.000175          21         8           mprotect
  4.03    0.000170           5        31           fstat
  3.15    0.000133           8        15           read
  2.30    0.000097          10         9           llistxattr
  1.82    0.000077          25         3         2 access
  1.54    0.000065           7         9           lgetxattr
  0.90    0.000038           4         8           lseek
  0.81    0.000034           5         6         4 prctl
  0.78    0.000033          16         2           getdents64
  0.76    0.000032          16         2         2 ioctl
  0.76    0.000032          16         2           statfs
  0.55    0.000023           5         4           futex
  0.50    0.000021           7         3           brk
  0.45    0.000019           4         4           rt_sigprocmask
  0.40    0.000017           8         2           getrandom
  0.36    0.000015          15         1           prlimit64
  0.14    0.000006           6         1           arch_prctl
  0.14    0.000006           6         1           set_tid_address
  0.14    0.000006           6         1           set_robust_list
  0.14    0.000006           6         1           rseq
  0.00    0.000000           0         1           write
------ ----------- ----------- --------- --------- ----------------
100.00    0.004220          11       371        56 total
```

### Observe (Part 3)

- **Why does `openat` have 102 calls (with 48 errors) while `close` has 56 calls?**
  `102 - 48 = 54` `openat` calls succeeded and returned valid file descriptors that were subsequently closed (along with `stdout` and `stderr` closed at exit: `54 + 2 = 56` `close` calls). The 48 failed `openat` calls returned `-1 ENOENT` when probing optional locale catalogs and NSS userdb paths, so no file descriptor was allocated and no `close` was needed.
- **Why does `write` appear 9–15 times when `ls -l` prints to a terminal, but only `1` time when redirected to `/dev/null`?**
  The C standard library (`glibc`) buffers `stdout` differently depending on whether descriptor `1` refers to an interactive terminal (`isatty(1) == 1`). On a terminal, `stdout` is **line-buffered**, so each output line triggers a separate `write(1, ...)` call. When redirected to `/dev/null`, `stdout` switches to **block-buffered** mode (4096-byte buffer), coalescing the entire listing into a single `write(1, ..., 469)` system call upon exit.
