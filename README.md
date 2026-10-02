# Operating Systems laboratory work

Ubuntu Linux and xv6 laboratory work, FCIM / FAF, UTM, 2026–2027.

## Repository structure

```text
README.md
lab1&2/
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

Labs 1 and 2 share a directory. Add subsequent labs as `lab5/lab5.md`, and so on when assigned; the final lab number is not yet known. All commits go directly to `main`.

## Reports

- [Lab 1 — The OS as a Resource Manager](lab1%262/report.md)
- [Lab 2 — Meet the OS You Will Build](lab1%262/notes.md)
- [Screenshot review and outstanding evidence](lab1%262/screenshot-review.md)
- [Lab 3](lab3/lab3.md)
- [Lab 4](lab4/lab4.md)

Reports use observed values from 23 supplied screenshots. Lab 2 booting and execution of the custom sleep program are demonstrated. The valid call returns to the prompt, and the no-argument call prints the expected usage message. The follow-up screenshot confirms that `sleep 10 20` also prints the expected usage message, completing both argument-count checks. See the review for details. LaTeX/PDF reports are a subsequent deliverable; these Markdown files retain the handout's requested report names.

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
