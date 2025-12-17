#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

// Mapeo de IDs numéricos a nombres legibles para la impresión en pantalla
static char *syscall_names[] = {
  [SYS_fork]    "fork",
  [SYS_exit]    "exit",
  [SYS_wait]    "wait",
  [SYS_pipe]    "pipe",
  [SYS_read]    "read",
  [SYS_kill]    "kill",
  [SYS_exec]    "exec",
  [SYS_fstat]   "fstat",
  [SYS_chdir]   "chdir",
  [SYS_dup]     "dup",
  [SYS_getpid]  "getpid",
  [SYS_sbrk]    "sbrk",
  [SYS_sleep]   "sleep",
  [SYS_uptime]  "uptime",
  [SYS_open]    "open",
  [SYS_write]   "write",
  [SYS_mknod]   "mknod",
  [SYS_unlink]  "unlink",
  [SYS_link]    "link",
  [SYS_mkdir]   "mkdir",
  [SYS_close]   "close",
  [SYS_trace]   "trace",
  [SYS_psinfo]  "psinfo",
  [SYS_fsinfo]  "fsinfo",
  [SYS_getcount] "getcount"
};

#define MAX_SYSCALL_ID 25

int
main(int argc, char *argv[])
{
  // CASO 1: Consulta individual por ID (Ejemplo: sysstat 5)
  if (argc == 2) {
    int id = atoi(argv[1]); // Convierte el argumento a entero
    int count;

    if (id <= 0 || id > MAX_SYSCALL_ID) {
      printf(2, "ID no válido. Rango: 1-%d\n", MAX_SYSCALL_ID);
      exit();
    }
    
    // Invocación a la nueva syscall implementada en el kernel
    count = getcount(id); 

    if (count < 0) {
      printf(2, "Error al consultar ID %d.\n", id);
    } else {
      char *name = (id <= MAX_SYSCALL_ID && syscall_names[id]) ? syscall_names[id] : "unknown";
      printf(1, "Syscall %d (%s) invocada %d veces.\n", id, name, count);
    }
  } 
  
  // CASO 2: Resumen global (Ejemplo: sysstat sin argumentos)
  else if (argc == 1) {
    printf(1, "--- Resumen de Invocaciones de Syscalls ---\n");
    printf(1, "ID | Nombre        | Contador\n");
    printf(1, "---|---------------|---------\n");

    // Itera sobre todas las syscalls para obtener sus contadores
    for (int id = 1; id <= MAX_SYSCALL_ID; id++) {
      int count = getcount(id);
      
      // Muestra solo las syscalls que han sido usadas al menos una vez
      if (count > 0) {
        char *name = (id <= MAX_SYSCALL_ID && syscall_names[id]) ? syscall_names[id] : "unknown";
        printf(1, "%2d | %-13s | %d\n", id, name, count);
      }
    }
    printf(1, "-------------------------------------------\n");
  } 
  
  // Manejo de error en sintaxis de comando
  else {
    printf(2, "Uso: sysstat [ID opcional]\n");
  }
  
  exit();
}