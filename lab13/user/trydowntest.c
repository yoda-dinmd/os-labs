// trydowntest.c: test for optional sem_trydown(id) (syscall 27).
#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  sem_init(4, 1);
  printf("first sem_trydown(4): %d (expected 0)\n", sem_trydown(4));
  printf("second sem_trydown(4): %d (expected -1)\n", sem_trydown(4));
  sem_up(4);
  printf("after sem_up, sem_trydown(4): %d (expected 0)\n", sem_trydown(4));
  exit(0);
}
