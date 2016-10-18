#ifndef _PREEMPTIVE_H
#define _PREEMPTIVE_H

#include "Process_struct.h"

int RR(FILE *out,struct PCB **processes,int count,int time_quantum);
int get_process_preemptive(struct node *Queue,struct PCB **processes,int current_time,char string[100]);
int Q_Assignment(struct PCB **processes,int pid,int no_of_queues);
void MLQ(FILE *out,struct PCB **processes,int count);
void MLFQ(FILE *out,struct PCB **processes,int count);

#endif