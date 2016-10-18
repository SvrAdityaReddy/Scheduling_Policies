#ifndef _PRINT_H
#define _PRINT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process_struct.h"

int print_process_schedule(FILE *out,struct node *process_schedule,struct PCB **processes);
void print_process(FILE *out,struct PCB *process,int *current_time,int *waiting_time,int *finish_time);
void print_schedulingpolicies();

#endif