#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "sched.h"
#include "math.h"

schedulerClass = 0;	// setting the default scheduler class as 0 


void setschedclass(int class){
	schedulerClass  = class; // setting the scheduler class
}

int getschedclass(){
	return(schedulerClass ); // getting the scheduler class
}


int getExpDistProc(int currExpDistProc){
	if(currExpDistProc < NPROC) // if there is an process available in the list of EXP DIST Scheduler
		return(dequeue(currExpDistProc));
	else 
		return(EMPTY);
}


int expDistNextProc(int priority){
	int nextProcess=q[rdytail].qprev, prevProcess=q[nextProcess].qprev;

	if(priority < q[prevProcess].qkey){ 
		if(prevProcess < NPROC){
			if(q[nextProcess ].qkey != q[prevProcess].qkey){
				nextProcess  = prevProcess;
			}
			prevProcess = q[prevProcess].qprev;
		}
	}
	
	if(nextProcess > NPROC)
	return (NULLPROC);
	
	else
	return nextProcess;
}
