#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "processInfo.h"

extern struct ptable ptable;

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

int
sys_hello(void)
{
	cprintf("hello xv6\n");
	return 0;
}

int
sys_hello_name(void)
{
	char *buffer;
	if (argstr(0, &buffer) < 0)
		return -1;
	cprintf("hello %s\n", buffer);
	return 0;
}

int
sys_get_num_proc(void)
{
	acquire(&ptable.lock);
	int count = 0;
	for (int i = 0; i < NPROC; i++) {
		if (ptable.proc[i].state != UNUSED)
			count++;
	}
	release(&ptable.lock);
	return count;
}

int
sys_get_max_pid(void)
{
	acquire(&ptable.lock);
	int maxpid = 0;
	for (int i = 0; i < NPROC; i++) {
		if (ptable.proc[i].pid > maxpid)
			maxpid = ptable.proc[i].pid;
	}
	release(&ptable.lock);
	return maxpid;
}

int
sys_get_proc_info(void)
{
	int pid;
	int index = -1;
	char *cp;
	struct processInfo *p;
	int size = sizeof(struct processInfo);

	if (argint(0, &pid) < 0)
		return -1;

	if (argptr(1, &cp, size) < 0)
		return -1;

	acquire(&ptable.lock);
	p = (struct processInfo*) cp;
	for (int i = 0; i < NPROC; i++) {
		if (ptable.proc[i].pid == pid) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		release(&ptable.lock);
		return -1;
	}

	if (ptable.proc[index].parent == 0)
		p->ppid = 0;
	else
		p->ppid = ptable.proc[index].parent->pid;
	p->psize = (int) ptable.proc[index].sz;
	p->numberContextSwitches = ptable.proc[index].count_context_switch;
	release(&ptable.lock);
	return 0;
}

int
sys_set_prio(void)
{
	int priority;
	if (argint(0, &priority) < 0)
		return -1;

	if (priority < 0)
		return -1;

	myproc()->priority = priority;
	return 0;
}

int
sys_get_prio(void)
{
	return myproc()->priority;
}
