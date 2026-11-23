# Lab 17 — Threads Share Memory, and Race

## Part 1 — Two workers

I compiled `fire.c` with `gcc -Wall -Wextra -pthread -o fire fire.c` and ran `./fire` several times. Each worker printed its initial observation of the shared counter and then attempted 1,000,000 increments. The main thread joined both workers before printing the final value. [Run transcript](evidence/two-threads.txt)

**What does it mean when thread 2 sees a non-zero counter?** In one run, thread 1 saw zero and thread 2 saw 19,110 before doing its own increments. Thread 2 could see changes already made by thread 1 because both threads access the same global variable in the process's address space. In the first three runs both workers read zero at startup, which can happen when both initial reads occur before the first increment.

**Why is the final value lower than 2,000,000 and why does it vary?** An increment reads the current value, adds one, and writes the result. If both workers read 100 before either stores, they each compute 101 and write 101, losing one increment. The scheduling and overlap of these operations vary. In C the unsynchronized accesses constitute a data race and have undefined behavior, so the lost-update example explains the observed runs without guaranteeing a particular result.

## Part 2 — Four workers

I used `./fire 4` to keep the work per thread the same and increase the expected total to 4,000,000. [Run transcript](evidence/four-threads.txt)

| Workers | Expected | Final counter | Lost updates |
| ---: | ---: | ---: | ---: |
| 2 | 2,000,000 | 1,079,270 | 920,730 |
| 2 | 2,000,000 | 1,080,160 | 919,840 |
| 2 | 2,000,000 | 1,070,093 | 929,907 |
| 2 | 2,000,000 | 1,021,287 | 978,713 |
| 4 | 4,000,000 | 1,098,403 | 2,901,597 |
| 4 | 4,000,000 | 1,114,100 | 2,885,900 |
| 4 | 4,000,000 | 1,069,228 | 2,930,772 |

All three four-worker runs lost more updates than every recorded two-worker run. About 72–73% of the expected increments were lost with four workers, compared with about 46–49% with two.

**What single idea would stop the lost updates?** Use one mutex to protect every access to the shared counter while the workers are active, holding it across the entire read-add-write of each increment.
