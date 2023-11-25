#include <kernel/types.h>
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  uint pid;
  int p[2]; // child process will also get a p copy
  char buf[4];
  pipe(p);

  pid = fork();

  if (pid == 0) {
    read(p[0], buf, 4);
    printf("%d: received ping\n", pid);
    close(p[0]);

    write(p[1], "pong", 4);
    close(p[1]);
  } else {
    write(p[1], "ping", 4);
    close(p[1]);

    read(p[0], buf, 4);
    printf("%d: received pong\n", pid);
    close(p[0]);
  }
  exit(0);
}
