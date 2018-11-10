#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <sched.h>

int ready(int pid, int resch)
{
	register struct	pentry	*pptr;
	pptr = &proctab[pid];
	pptr->pstate = PRREADY;

	if(schedulerClass == LINUXSCHED)
	{
		insert(pid,rdyhead,pptr->pprio + pptr->counter);
	}
	else 
	{
		insert(pid,rdyhead,pptr->pprio);
	}
	if (resch)
	{
		resched();
	}
	return(OK);
}
