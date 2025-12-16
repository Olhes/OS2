#include "types.h"
#include "user.h"
#include "stat.h"

int main(void) {
  trace(0);  // Disable tracing
  printf(1, "System call tracing disabled\n");
  exit();
}
