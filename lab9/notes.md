# Lab 9 observations

1. `ls` uses `write(1, ...)` to print the listing. Descriptor 1 is standard output.
2. Descriptors 0, 1 and 2 are standard input, standard output and standard error. They are already open, so `open` chooses descriptor 3, the lowest free descriptor.
3. The filtered `strace -f -e trace=openat,read,write,close ls -l 2>&1 > /dev/null | wc -l` run counted 187 displayed calls. This count varies with environment and covers only the selected call types.
4. The `ENOENT` results shown for optional locale catalogs are failed file probes. `ls -l` is not recursive; the trace's later reads are for the current directory listing.

The direct system-call program's output and filtered trace excerpt are recorded in the report; screenshots are `pic1.png` and `pic2.png`.
