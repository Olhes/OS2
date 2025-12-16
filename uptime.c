#include "types.h"
#include "user.h"
#include "stat.h"

int main(void) {
  uint ticks = uptime();
  printf(1, "%d\n", ticks);
  exit();
}
