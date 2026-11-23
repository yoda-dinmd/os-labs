#include "kernel/types.h"
#include "user/user.h"

#define FREE        0
#define RUNNING     1
#define RUNNABLE    2
#define STACK_SIZE  8192
#define MAX_THREAD  4

// Keep this order in sync with the offsets in uthread_switch.S.
struct context {
  uint64 ra;
  uint64 sp;
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

struct thread {
  struct context ctx;
  char stack[STACK_SIZE] __attribute__((aligned(16)));
  int state;
};

static struct thread all_thread[MAX_THREAD];
static struct thread *current_thread;

extern void thread_switch(struct context *old, struct context *next);

static void
thread_init(void)
{
  current_thread = &all_thread[0];
  current_thread->state = RUNNING;
}

static void
thread_schedule(void)
{
  struct thread *next = 0;
  struct thread *t = current_thread + 1;

  for (int i = 0; i < MAX_THREAD; i++, t++) {
    if (t == all_thread + MAX_THREAD)
      t = all_thread;
    if (t->state == RUNNABLE) {
      next = t;
      break;
    }
  }

  if (next == 0) {
    printf("thread_schedule: no runnable threads\n");
    exit(0);
  }

  struct thread *old = current_thread;
  next->state = RUNNING;
  current_thread = next;
  if (old != next)
    thread_switch(&old->ctx, &next->ctx);
}

static void
thread_create(void (*func)(void))
{
  struct thread *t;

  for (t = all_thread; t < all_thread + MAX_THREAD; t++) {
    if (t->state == FREE)
      break;
  }
  if (t == all_thread + MAX_THREAD) {
    fprintf(2, "thread_create: no free slots\n");
    exit(1);
  }

  t->ctx = (struct context){
    .ra = (uint64)func,
    .sp = (uint64)(t->stack + sizeof(t->stack)),
  };
  t->state = RUNNABLE;
}

static void
thread_yield(void)
{
  current_thread->state = RUNNABLE;
  thread_schedule();
}

static volatile int a_started, b_started, c_started;
static volatile int a_n, b_n, c_n;

static void
thread_run(const char *name, volatile int *started, volatile int *count,
           const volatile int *other_started, const volatile int *last_started)
{
  printf("%s started\n", name);
  *started = 1;
  while (*other_started == 0 || *last_started == 0)
    thread_yield();

  for (int i = 0; i < 100; i++) {
    printf("%s %d\n", name, i);
    *count += 1;
    thread_yield();
  }
  printf("%s: exit after %d\n", name, *count);

  current_thread->state = FREE;
  thread_schedule();
  // A finished thread has no caller and must never return.
  exit(1);
}

static void
thread_a(void)
{
  thread_run("thread_a", &a_started, &a_n, &b_started, &c_started);
}

static void
thread_b(void)
{
  thread_run("thread_b", &b_started, &b_n, &a_started, &c_started);
}

static void
thread_c(void)
{
  thread_run("thread_c", &c_started, &c_n, &a_started, &b_started);
}

int
main(void)
{
  thread_init();
  thread_create(thread_a);
  thread_create(thread_b);
  thread_create(thread_c);
  current_thread->state = FREE;
  thread_schedule();
  exit(1);
}
