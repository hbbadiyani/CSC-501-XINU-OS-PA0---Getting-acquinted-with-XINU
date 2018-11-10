#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "math.h"
#include "sched.h"
#include <stdio.h>

#define LAMBDA 0.1

unsigned long currSP;	
extern int ctxsw(int, int, int, int);



int resched()
{
	register struct	pentry	*optr , *nptr;
	int randomP;				
	int nextProcess, pVal=0;				
	int optrPriority;				

	if (schedulerClass == LINUXSCHED) {
		pVal++;
		int gMax = 0, nextProcess = 0, temp = 0;
		optr = &proctab[currpid];

		int i = 0, j = optr->counter;
		temp = optr->goodness;		
		optrPriority = temp - j;
		temp = optrPriority;

		if(preempt!=0)
			optr->goodness = temp + preempt;
					
		optr->counter = preempt;
		
		if(optr->counter <=0)
		{
			optr->counter = 0;
			optr->goodness = 0;
		}
		if (currpid == NULLPROC ){
			optr->counter = 0;
			optr->goodness = 0;
		}
		int procTemp = q[rdytail].qprev;
		while(procTemp != rdyhead){
			while(proctab[procTemp].goodness > gMax)
			{
				nextProcess = procTemp;
				gMax = proctab[procTemp].goodness;
			}
			procTemp = q[procTemp].qprev;
		}

		if (optr->counter == 0 && gMax == 0) 
		{			
			int currP = 0;
			int pVal = 1;
			struct pentry *ptr;
			
			while(currP < NPROC){
				
				int nextPtr = 0;
				nextPtr+=currP;
				
				
				ptr = &proctab[currP];
				if (ptr->pstate != PRFREE){
					ptr->counter =  (int)((ptr->counter)*3)/2;
					ptr->counter +=ptr->pprio;
					pVal++;
				}
				ptr->goodness =   ptr->counter;
				ptr->goodness += ptr->pprio;
				currP += 1;				
			}
			if(pVal>0)
			preempt = optr->counter;
			
			switch(currpid)
			{
				case NULLPROC:
					return OK;
					break;
				default:
					#ifdef	RTCLOCK
					preempt = QUANTUM;
					#endif
			}			
		}
		else if (optr->goodness > 0 && optr->goodness >= gMax){
			if (optr->pstate != PRCURR)
			{
				temp = optr->counter;
				i = (int)(temp*3)/2;
			}
			else 			
			{
				preempt = optr->counter;
				return(OK);				
			}
		}		
		if (optr->pstate == PRCURR) {
				optr->pstate = PRREADY;
				insert(currpid, rdyhead, optr->pprio);
		}

		nptr = &proctab[nextProcess];
		nptr->pstate = PRCURR;
		dequeue(nextProcess);
		currpid = nextProcess;
		preempt = nptr->counter;
		
		ctxsw((int) &optr->pesp, (int) optr->pirmask, (int) &nptr->pesp, (int) nptr->pirmask);
		return(OK);

	}

    else if (EXPDISTSCHED == schedulerClass){
		
		pVal++;

		randomP = (int) expdev(LAMBDA);
		nextProcess = expDistNextProc(randomP);
		optr = &proctab[currpid];
		//checking if preemption counter needs to be reset or not
		//Preemption is required if 
		//1. Priority of next process is higher than the old process
		//2. The random value is lower than the priority of old process
		//3. The old process state is PRCURR
		if (optr->pstate == PRCURR && (q[nextProcess].qkey > optr->pprio && optr->pprio > randomP)){
			#ifdef	RTCLOCK
				preempt = QUANTUM;
			#endif
			return(OK);
		}			
	}
	else {
		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
			(lastkey(rdytail) < optr->pprio)) {
			return(OK);
		}
	}

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	

	// deciding value of currpid based on the scheduler type
	// getExpDistProc(nextProcess) for EXPDISTSCHED && getlast(rdytail) for LINUXSCHED or else
	currpid = schedulerClass == EXPDISTSCHED ? getExpDistProc(nextProcess) : getlast(rdytail);
	
	
	if(pVal>=0)
	{
		nptr = &proctab[currpid];
		nptr->pstate = PRCURR;		
		#ifdef	RTCLOCK
			preempt = QUANTUM;		
		#endif
	}
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);

	return OK;
}
