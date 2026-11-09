# Lab 9 Observations (`strace`)

## Goal
Observe the system-call boundary between user-space programs and the Linux kernel using `strace`: trace a standard utility (`ls -l`), write and trace a C program (`syscalls.c`) that invokes `open`, `write`, `read`, and `close` directly, and count individual system call types using `strace -c` and `strace -e trace=<syscall> ... | grep -c`.

## Part 1 — See the calls of a real command (`ls -l`)
- **Command:** `strace -f -e trace=openat,read,write,close ls -l`
- **Observe:**
  1. `ls` uses the `write` system call (`write(1, ..., ...)`) to print the directory listing to the screen.
  2. It writes to **descriptor `1`**, which is **standard output (`stdout`)**.
  3. Across the four filtered system calls (`openat,read,write,close`), `ls -l` makes roughly **174–186 calls** (and **368–371 total system calls** when unfiltered). Many `openat` calls returning `-1 ENOENT` are normal probes for optional locale catalogs (`/usr/share/locale/...`) and NSS user/group databases (`/etc/userdb/...`).

## Part 2 — A program that calls the kernel directly (`syscalls.c`)
- **Build & run:** `gcc -Wall -Wextra -o syscalls syscalls.c && ./syscalls`
- **Trace command:** `strace -e trace=openat,write,read,close ./syscalls`
- **Observe:**
  - The first `openat("note.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644)` returns **descriptor `3`** (not `0`, `1`, or `2`) because descriptors `0` (standard input), `1` (standard output), and `2` (standard error) are already open and inherited from the shell. Linux always allocates the lowest unused file descriptor, which is `3`. After `close(3)`, descriptor `3` becomes free again and is reused by `openat("note.txt", O_RDONLY)`.

## Part 3 — Count one kind of call
- **Counting a single system call type with `strace -e trace=... | grep -c`:**
  - `strace -e trace=openat ls -l 2>&1 >/dev/null | grep -c '^openat'` → **102** calls
  - `strace -e trace=close ls -l 2>&1 >/dev/null | grep -c '^close'` → **56** calls
  - `strace -e trace=read ls -l 2>&1 >/dev/null | grep -c '^read'` → **15** calls
  - `strace -e trace=write ls -l 2>&1 >/dev/null | grep -c '^write'` → **1** call when redirected to `/dev/null` (block-buffered `stdout`), or **9–15** calls when writing directly to a terminal (line-buffered `stdout`, one `write` per line).
- **Counting per system call type with `strace -c`:**
  - `strace -c -e trace=openat,read,write,close ls -l >/dev/null` reports `102` `openat` (48 errors), `56` `close`, `15` `read`, and `1` `write` (total `174` calls across those four types).
  - `strace -c ls -l >/dev/null` reports the breakdown across all 27 invoked syscall types (`371` total calls).
  - Note: Piping `strace -f -e trace=openat,read,write,close ls -l 2>&1 >/dev/null | wc -l` counts all displayed trace lines across all four call types plus the exit line (`187` lines in the terminal run), whereas `strace -c` or `strace -e trace=<call> ... | grep -c` counts a single kind of system call.
