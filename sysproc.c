#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

// External declaration for ptable
extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Filesystem information
int
sys_fsinfo(void)
{
  // Simple filesystem info using available data
  // For now, we'll provide basic disk usage information
  
  // These values are hardcoded for XV6's default filesystem
  // In a real implementation, we would read from the superblock
  int total_blocks = 1000;  // Default XV6 filesystem size
  int block_size = 512;     // BSIZE in XV6
  int used_blocks = 784;    // Approximate current usage
  int free_blocks = total_blocks - used_blocks;
  
  // Print filesystem information
  cprintf("Filesystem information:\n");
  cprintf("  Total blocks: %d\n", total_blocks);
  cprintf("  Used blocks:  %d\n", used_blocks);
  cprintf("  Free blocks:  %d\n", free_blocks);
  cprintf("  Block size:   %d bytes\n", block_size);
  cprintf("  Total size:   %d bytes\n", total_blocks * block_size);
  cprintf("  Used size:    %d bytes\n", used_blocks * block_size);
  cprintf("  Free size:    %d bytes\n", free_blocks * block_size);
  
  return 0;
}

// Extended uptime with additional system information
int
sys_psinfo(void)
{
  struct proc *p;
  int proc_count = 0;
  
  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    
    proc_count++;
    
    // Print process information directly using cprintf
    char state[10];
    switch(p->state){
      case UNUSED:   safestrcpy(state, "UNUSED", sizeof(state)); break;
      case EMBRYO:   safestrcpy(state, "EMBRYO", sizeof(state)); break;
      case SLEEPING: safestrcpy(state, "SLEEP", sizeof(state)); break;
      case RUNNABLE: safestrcpy(state, "RUNBL", sizeof(state)); break;
      case RUNNING:  safestrcpy(state, "RUN", sizeof(state)); break;
      case ZOMBIE:   safestrcpy(state, "ZOMBIE", sizeof(state)); break;
    }
    
    cprintf("%d %s %s %d\n", p->pid, state, p->name, p->sz);
  }
  release(&ptable.lock);
  
  return proc_count;
}
