# Lab 6 — Inside the xv6 File System

**Submitted report: [Lab 6 PDF](../reports/lab6.pdf).**

This laboratory examines xv6 directory entries, inode metadata, hard links and filesystem limits. It includes two user programs: `mystat` for metadata inspection and recursive `find` with the optional `-n` inode display.

| Material | Location |
|---|---|
| Compiled report | [../reports/lab6.pdf](../reports/lab6.pdf) |
| LaTeX report source | [lab6.tex](lab6.tex) |
| Written observations | [notes.md](notes.md) |
| Required programs | [user/mystat.c](user/mystat.c) · [user/find.c](user/find.c) |
| Build integration | [Makefile](Makefile) · [Makefile.patch](Makefile.patch) |
| Build and execution evidence | [evidence/](evidence/) |
| Validation results | [evidence/checks.txt](evidence/checks.txt) |

The programs were built and tested on xv6 revision `06aad25c735fd3159bdfae5680be4eab7b1668b2`. Tests cover the required fresh-image examples, hard-link counts, missing paths, full-length directory names, deleted directory slots, 19 nested directories and 24 sibling directories.

The full Makefile retains the earlier `args` and `sleep` utilities; their sources are included in `user/` for a complete build. The focused patch applies to the corresponding Lab 4 Makefile.

Setup and compilation instructions are available in the [repository README](../README.md#reproducing-program-experiments).
