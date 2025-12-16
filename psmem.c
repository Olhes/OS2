#include "types.h"
#include "user.h"
#include "stat.h"

#define MAX_PS 4096

int main(void) {
  printf(1, "PID   STATE  NAME         SIZE\n");
  printf(1, "-----------------------------\n");
  
  // Call simplified psinfo that prints directly
  if (psinfo() < 0) {
    printf(2, "psinfo failed\n");
    exit();
  }
  
  exit();
}
