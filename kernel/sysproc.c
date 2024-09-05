#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va;
  int pg_num;
  uint64 buf_addr;
  uint64 buf = 0;
  
  struct proc* p = myproc();

  argaddr(0, &va);  // get the start virtual address
  argint(1, &pg_num); // get the numver of pages to check
  argaddr(2, &buf_addr);  // get the user address to a buffer to store the result

  va = PGROUNDDOWN(va);
  for (int i = 0; i < pg_num; i++) {
    if ((va + i * PGSIZE) > MAXVA) {
      return -1;
    }
    pte_t* pte = walk(p->pagetable, va + (i * PGSIZE), 0);
    if (pte == 0) {
      printf("pte don't exit\n");
      exit(1);
    }
    if (*pte & PTE_A) {
      buf  = buf | (1L << i);
    }
    *pte &= ~PTE_A;
  }
  copyout(p->pagetable, buf_addr, (char*)&buf, sizeof(buf));
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
