

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <bsd/stdlib.h>
#include "Process_struct.h"
#include "Sort.h"
#include "Print.h"
#include "Non_preemptive.h"
// Process-id AT ST Pr(if any) FT TAT mean-TAT Ini-WT Inter-WT TWT mean-WT





void Initialize(struct PCB **processes,int count)
	{
	int i=0;
	for (i=0;i<count;i++)
		{
		processes[i]->rst=processes[i]->st;
		}
	}

int main(int argc, char** argv)
	{
    struct PCB **processes; 
	FILE *in;
	//in = fopen("test.dat","r+w");
	in = fopen(argv[1],"r+w");
	int i=0,count=0;
	char string[100];
    // To count number of processes
	while (!feof(in))
		{
		fscanf(in,"%s\n",string);
		count++;
		}
	fclose(in);
	count = count/4;
    processes = (struct PCB **)malloc(sizeof(struct PCB*)*count);
    for(i=0;i<count;i++)
		{
		processes[i]=(struct PCB *)malloc(sizeof(struct PCB));	
		}
	i=0;
	//in=fopen("test.dat","r+w");
	in = fopen(argv[1],"r+w");
	while(!feof(in))
		{
		fscanf(in,"%s",processes[i]->pid);
		fscanf(in,"%d\n",&(processes[i]->at));
		fscanf(in,"%d\n",&(processes[i]->st));
		fscanf(in,"%d\n",&(processes[i]->priority));
		processes[i]->rst=processes[i]->st;
		processes[i]->wt=0;
		i++;
		}
	FILE *out;
	out = fopen("out.dat","w+");
	sort(processes,count);
	FCFS(out,processes,count);
	Initialize(processes,count);
	SJF(out,processes,count);
	Initialize(processes,count);
	Priority(out,processes,count);
	Initialize(processes,count);
	HRRN(out,processes,count);
	Initialize(processes,count);
	SRTN(out,processes,count);
	Initialize(processes,count);
	RR(out,processes,count,2);
	Initialize(processes,count);
	MLQ(out,processes,count);
	Initialize(processes,count);
	MLFQ(out,processes,count);
	return 0;
	}
