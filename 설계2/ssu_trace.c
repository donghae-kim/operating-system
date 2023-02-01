#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

int
main(int argc, char *argv[])
{
  
  char *tmp_argv[32]; //argv 최대치 32여서 32
  int i;
  
  if (trace(atoi(argv[1])) < 0) {
    printf(1, "%s: trace 실패했습니다.\n", argv[0]);
    exit();
  }
  
  for(i = 2; i < argc && i < 32; i++){
    tmp_argv[i-2] = argv[i];
  }

  exec(tmp_argv[0], tmp_argv);
  exit();
}