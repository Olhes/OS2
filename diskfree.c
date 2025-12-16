#include "types.h"
#include "user.h"
#include "stat.h"

int main(void) {
  printf(1, "Disk Usage Information:\n");
  printf(1, "=====================\n");
  
  if (fsinfo() < 0) {
    printf(2, "diskfree: failed to get filesystem info\n");
    exit();
  }
  
  exit();
}
