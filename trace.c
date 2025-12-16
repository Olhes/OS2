#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(2, "Usage: %s command [args...]\n", argv[0]);
    exit();
  }

  // Enable tracing
  trace(1);

  // Execute the command
  exec(argv[1], &argv[1]);
  printf(2, "exec %s failed\n", argv[1]);
  
  // Disable tracing if exec failed
  trace(0);
  exit();
}
