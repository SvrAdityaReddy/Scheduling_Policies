#ifndef _NON_PREEMPTIVE_H
#define _NON_PREEMPTIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process_struct.h"

int FCFS(FILE *out,struct PCB **processes,int count);
int Non_preemptive(FILE *out,struct PCB **processes,int count,char string[100]);
int SJF(FILE *out,struct PCB **processes,int count);
int Priority(FILE *out,struct PCB **processes,int count);
int HRRN(FILE *out,struct PCB **processes,int count);
int SRTN(FILE *out,struct PCB **processes,int count);

#endif