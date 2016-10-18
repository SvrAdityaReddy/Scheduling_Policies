#include "Print.h"

int print_process_schedule(FILE *out,struct node *process_schedule,struct PCB **processes)
	{
	struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp=process_schedule;
    while(temp!=NULL)
        {
        fprintf(out,"%s ",processes[temp->value]->pid);
        temp=temp->next;
        }
	fprintf(out,"%s","\n");
    return 0;
	}

void print_process(FILE *out,struct PCB *process,int *current_time,int *waiting_time,int *finish_time)
	{
	fprintf(out,"%s ",process->pid);
	*finish_time = *current_time + process->rst;
	fprintf(out,"%d ",process->at);
	fprintf(out,"%d ",process->st);
	fprintf(out,"%d ",*finish_time);
	fprintf(out,"%d ",*finish_time-process->at);
    *waiting_time=*finish_time-process->at-process->st;
	fprintf(out,"%d\n",*waiting_time);
	*current_time = *finish_time;
	}



void print_schedulingpolicies()
	{
	printf("List of Scheduling policies are:\n");
	printf("1. FCFS\n");
	printf("2. SJF\n");
	printf("3. HRRN\n");
	printf("4. Priority\n");
	printf("5. SRTN\n");
	printf("6. RR\n");
	}