#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *pattern){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(read(fd, &de, sizeof(de)) != sizeof(de)){
    fprintf(2, "find: cannot read %s", path);
    exit(1);
  }

  switch (st.type){
    case T_DEVICE:
    case T_FILE:
      if(strcmp(de.name, pattern) == 0) {
        printf("%s/%s\n", path, pattern);
      }
      break;

    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2, "find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      do {
        if((de.inum == 0) || strcmp(de.name, ".") == 0 || (strcmp(de.name, "..") == 0))
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
          fprintf(2, "find: cannot state %s\n", buf);
          continue;
        }

        if (st.type == T_FILE){
          if (strcmp(de.name, pattern) == 0){
            printf("%s\n", buf);
          }
        } else if(st.type == T_DIR){
          find(buf, pattern);
        }
      } while(read(fd, &de, sizeof(de)) == sizeof(de));
      break;
    }
    close(fd);
}


int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Error: please give file you need to find");
    exit(1);  // NOTE: you need to give none zero return value
  }
  find(argv[1], argv[2]);  
  exit(0);
}
