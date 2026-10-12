#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define PATHSIZE 512
// Snapshot names so the directory can be closed before recursion.
struct entry {
  struct entry *next;
  char name[DIRSIZ + 1];
};
static int failed;

static void
find(char *path, char *name, int numbers)
{
  struct stat st;
  struct dirent de;
  struct entry *head = 0, **tail = &head, *e;
  char *base, *child;
  int fd, n;
  uint len = strlen(path);

  if (stat(path, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    failed = 1;
    return;
  }
  base = path + len;
  while (base > path && base[-1] != '/')
    base--;
  if (strcmp(base, name) == 0) {
    if (numbers)
      printf("%s %d\n", path, st.ino);
    else
      printf("%s\n", path);
  }
  if (st.type != T_DIR)
    return;
  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    failed = 1;
    return;
  }
  while ((n = read(fd, &de, sizeof(de))) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    if ((de.name[0] == '.' && de.name[1] == 0) ||
        (de.name[0] == '.' && de.name[1] == '.' && de.name[2] == 0))
      continue;
    e = malloc(sizeof(*e));
    if (e == 0) {
      fprintf(2, "find: out of memory\n");
      failed = 1;
      break;
    }
    memmove(e->name, de.name, DIRSIZ);
    e->name[DIRSIZ] = 0;
    e->next = 0;
    *tail = e;
    tail = &e->next;
  }
  if (n != 0 && n != sizeof(de)) {
    fprintf(2, "find: cannot read directory %s\n", path);
    failed = 1;
  }
  close(fd);
  while ((e = head) != 0) {
    head = e->next;
    uint slash = len > 0 && path[len - 1] != '/';
    uint size = len + slash + strlen(e->name) + 1;
    if (size > PATHSIZE) {
      fprintf(2, "find: path too long below %s\n", path);
      failed = 1;
    } else if ((child = malloc(size)) == 0) {
      fprintf(2, "find: out of memory\n");
      failed = 1;
    } else {
      strcpy(child, path);
      if (slash)
        child[len] = '/';
      strcpy(child + len + slash, e->name);
      find(child, name, numbers);
      free(child);
    }
    free(e);
  }
}

int
main(int argc, char *argv[])
{
  if ((argc != 3 && argc != 4) ||
      (argc == 4 && strcmp(argv[3], "-n") != 0)) {
    fprintf(2, "usage: find <dir> <name> [-n]\n");
    exit(1);
  }
  // Normalize trailing slashes for basename comparisons, except for root.
  int len = strlen(argv[1]);
  while (len > 1 && argv[1][len - 1] == '/')
    argv[1][--len] = 0;
  find(argv[1], argv[2], argc == 4);
  exit(failed);
}
