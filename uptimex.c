#include "types.h"
#include "user.h"
#include "stat.h"

int main(void) {
  uint ticks;
  int num_procs = 0;
  
  // Get basic uptime
  ticks = uptime();
  
  // Get real process count from psinfo
  num_procs = psinfo();
  
  printf(1, "up %d ticks, %d processes\n", ticks, num_procs);
  exit();
}
