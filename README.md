# Operating Systems laboratory work

Ubuntu Linux and xv6 laboratory work, FCIM / FAF, UTM, 2026–2027.

Student: **Gafenco Victor**, group **FAF-241**. Course: **Operating Systems**.
Instructor: **Patricia Reitman**, Assistant lecturer.

Repository: [yoda-dinmd/os-labs](https://github.com/yoda-dinmd/os-labs).

## Repository structure

```text
README.md
metadata.tex             # Shared name, group, course, and instructor
build-reports.sh         # Compile reports with Tectonic
build/
  lab1.pdf
  lab2.pdf
latex/                   # Shared report class, setup, and UTM logo
lab1&2/
  lab1.tex               # Lab 1 LaTeX report
  lab2.tex               # Lab 2 LaTeX report
  report.md              # Lab 1 observations
  notes.md               # Lab 2 observations and implementation
  screenshot-review.md   # Review of every supplied screenshot
  screenshots/           # Original evidence for both labs
  user/sleep.c           # Transcribed from the source screenshot
  Makefile.patch         # Correct build registration for sleep
lab3/
  lab3.md
lab4/
  lab4.md
```

Labs 1 and 2 share a directory. Labs 5 and 6 use `lab5/report.md` and `lab6/notes.md`; the final lab number is not yet known. All commits go directly to `main`.

## Reports

- [Lab 1 — The OS as a Resource Manager](lab1%262/report.md)
- [Lab 2 — Meet the OS You Will Build](lab1%262/notes.md)
- [Lab 1 PDF](build/lab1.pdf) · [LaTeX source](lab1%262/lab1.tex)
- [Lab 2 PDF](build/lab2.pdf) · [LaTeX source](lab1%262/lab2.tex)
- [Screenshot review and outstanding evidence](lab1%262/screenshot-review.md)
- [Lab 3](lab3/lab3.md)
- [Lab 4](lab4/lab4.md)

Reports use observed values from 23 supplied screenshots. Lab 2 booting and execution of the custom sleep program are demonstrated. The valid call returns to the prompt, and the no-argument call prints the expected usage message. The follow-up screenshot confirms that `sleep 10 20` also prints the expected usage message, completing both argument-count checks. See the review for details. The Markdown files retain the handout's requested report names; the LaTeX reports use the supplied individual UTM report layout.

## Compile the PDF reports

Edit [metadata.tex](metadata.tex) to update personal or course details shared by all reports. From the repository root, run:

```bash
bash build-reports.sh
```

The script requires `tectonic` on your PATH and writes the final PDFs to `build/`. Tectonic may download missing packages on the first build. If your interactive shell defines `pdf` as shorthand for Tectonic, the equivalent commands are:

```bash
mkdir -p build
cd 'lab1&2'
pdf --outdir ../build lab1.tex
pdf --outdir ../build lab2.tex
```

PDFs are kept in Git for convenient access; LaTeX intermediate files are ignored. The files in `latex/` include the report class and English logo needed for compilation, so a separate template checkout is not required. The local `template/` folder is reference material.

## Run the Lab 2 program

Use the same xv6 checkout as the screenshots, whose `user/user.h` declares `int pause(int);`. The program is named `sleep` but calls `pause` in this checkout. The full xv6 tree is not included here.

From your xv6 checkout, copy this repository's `lab1&2/user/sleep.c` to `user/sleep.c`. Add a tab-indented `$U/_sleep\` entry to `UPROGS`, preserving a blank line before the `fs.img:` rule. `lab1&2/Makefile.patch` records that change for an otherwise unmodified matching Makefile; do not apply it if the entry already exists.

Run `make qemu`, then execute inside xv6:

```text
echo before
sleep 10
echo after
sleep
sleep 10 20
```

Both invalid argument counts should print `usage: sleep <ticks>`. Save the output as `lab1&2/screenshots/L2_11_sleep_test.png`. Exit QEMU with Ctrl+A, then X.

## Labs 5 and 6

- [Lab 5 report](lab5/report.md) · [LaTeX](lab5/lab5.tex) · [PDF](build/lab5.pdf)
- [Lab 6 report](lab6/notes.md) · [LaTeX](lab6/lab6.tex) · [PDF](build/lab6.pdf)

Lab 5 records the Linux filesystem experiments, including the actual Btrfs
directory link counts and the optional gzip/open-file exercises. Lab 6 includes
`mystat`, recursive `find`, optional `find -n`, and fresh-image QEMU results.
Both PDFs use the shared `metadata.tex` and existing UTM layout.

To run Lab 6, use xv6 revision `06aad25c735fd3159bdfae5680be4eab7b1668b2`
with the Lab 4 setup. Copy `lab6/user/mystat.c` and `lab6/user/find.c` to its
`user/` directory, then apply `lab6/Makefile.patch` from the xv6 root. The full
`lab6/Makefile` is also supplied and retains the earlier `args` and `sleep`
registrations. For a clean checkout, copy all four files from `lab6/user/`
and the full `lab6/Makefile`; the included `args.c` and `sleep.c` supply those
earlier utilities. Run `make qemu`; exit with Ctrl+A, then X.

`make clean` rebuilds the filesystem image and discards its previous contents.
Use a separate checkout for the fresh-disk experiments. The reports include
commands and output; full transcripts are under each lab's `evidence/`.
