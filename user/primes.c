#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child(int *p);

int main(int argc, char *argv[]) {
  int p[2];
  int start = 2;
  int end = 35;

  pipe(p);
  if (fork() == 0) {
    child(p);
  } else {
    close(p[0]);
    for (int i = start; i <= end; i++) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    wait((int *)0);
  }
  exit(0);
}

void child(int *p_father) {
  int n;
  int p_child[2];
  int prime;
  close(p_father[1]);
  read(p_father[0], &prime, sizeof(int));
  printf("prime %d\n", prime);
  pipe(p_child);
  // The following if is very important
  // useful in the last child process
  if (read(p_father[0], &n, sizeof(int))) {
    if (fork() == 0) {
      child(p_child);
    } else {
      close(p_child[0]);
      do {
        if (n % prime != 0) {
          write(p_child[1], &n, sizeof(int));
        }
      } while (read(p_father[0], &n, sizeof(int)));
      close(p_father[0]);
      close(p_child[1]);
      wait((int *)0);
    }
  }
  exit(0);
}
