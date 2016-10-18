#ifndef _HELPER_H
#define _HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process_struct.h"

int get_process(int *pid_array,struct PCB **processes,int count,int current_time,char string[100]);
int is_completed(int *completed_array,int count);

#endif