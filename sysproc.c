#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

//added argint 
int
sys_exit(void)
{
  int v; 
  argint(0, &v);
  exit(v); 
  return 0;  // not reached
}

//added agrptr 
int
sys_wait(void)
{
  int *status; 
  argptr(0, (char**) &status, sizeof(int)); 
  return wait(status);
}

//added agrptr and agrint to take in 3 parameters 
int 
sys_waitpid(void)
{
  int id, op; 
  int *status; 
  argint(0, &id); 
  argptr(1, (char**) &status, sizeof(int)); 
  argint(2, &op); 
  return waitpid(id, status, op); 
}

//added setpriority system call and used argint 
int 
sys_setpriority(void)
{
  int new_priority; 
  if (argint(0, &new_priority) < 0) 
     return -1; 
  return setpriority(new_priority); 
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
