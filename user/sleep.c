#include <kernel/types.h>
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]){
  if (argc > 2){
    write(2, "Wrong number of arguments!\n", 27);
    return -1;
  }

  for (int i = 0; i < atoi(argv[1]); i++){
    sleep(1);
  }
  exit(0);
}
