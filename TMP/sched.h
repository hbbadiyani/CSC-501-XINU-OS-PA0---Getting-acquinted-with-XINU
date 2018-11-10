#ifndef _SCHED_H_
#define _SCHED_H_

#define EXPDISTSCHED 1
#define LINUXSCHED 2

extern int schedulerClass;

extern void setschedclass(int sched_class);
extern int getschedclass();

extern int getExpDistProc(int expdist_proc);
extern int expDistNextProc(int random_priority);

#endif

