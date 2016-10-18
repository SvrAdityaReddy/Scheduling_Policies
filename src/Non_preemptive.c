#include "Non_preemptive.h"
#include "Helper.h"

int FCFS(FILE *out,struct PCB **processes,int count)
	{
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	fprintf(out,"%s\n","Policy is : FCFS");
	struct node *process_schedule;
	process_schedule=NULL;
	for(i=0;i<count;i++)
		{
		fprintf(out,"%s ",processes[i]->pid);
		current_time = finish_time;
		finish_time = finish_time + processes[i]->st;
		fprintf(out,"%d ",processes[i]->at);
		fprintf(out,"%d ",processes[i]->st);
		fprintf(out,"%d ",finish_time);
		fprintf(out,"%d ",finish_time-processes[i]->at);
		mean_TA_time=mean_TA_time+finish_time-processes[i]->at;
        waiting_time=finish_time-processes[i]->at-processes[i]->st;
		fprintf(out,"%d\n",waiting_time);
		mean_waiting_time=mean_waiting_time+waiting_time;
		createnode_at_end(&process_schedule,i);
		}
	mean_waiting_time=mean_waiting_time/((float)count);
	mean_TA_time=mean_TA_time/((float)count);
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	return 0;
	}

int Non_preemptive(FILE *out,struct PCB **processes,int count,char string[100])
	{
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	fprintf(out,"Policy is : %s\n",string);
	int no_of_processes=0;
	struct node *process_schedule;
	process_schedule=NULL;
	int *pid_array;
	pid_array=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		{
		pid_array[i]=-1;
		}
	int j=0;
	int *completed_pidarray;
	completed_pidarray=(int *)malloc(sizeof(int)*count);
	finish_time=processes[0]->at;
	for (j=0;j<count;j++)
		{
		if(processes[j]->at<=finish_time)
			{
			pid_array[j]=j;
			}
		}
	while(is_completed(completed_pidarray,count)!=1)
		{
		int min=0;
		min=get_process(pid_array,processes,count,current_time,string);
		if(min>=0)
			{
			print_process(out,processes[min],&current_time,&waiting_time,&finish_time);
			createnode_at_end(&process_schedule,min);
			mean_waiting_time=mean_waiting_time+waiting_time;
			mean_TA_time=mean_TA_time+(finish_time-processes[min]->at);
			//printf("%s ",processes[min]->pid);
			pid_array[min]=-1;
			completed_pidarray[min]=1;
			for (j=0;j<count;j++)
				{
				if(completed_pidarray[j]!=1)
					{
					if(processes[j]->at<=current_time)
						{
						pid_array[j]=j;
						}
					}
				}
			}
		else 
			{
			current_time=current_time+1;
			for (j=0;j<count;j++)
				{
				if(completed_pidarray[j]!=1)
					{
					if(processes[j]->at<=current_time)
						{
						pid_array[j]=j;
						}
					}
				}
			}						
		}
	mean_waiting_time=mean_waiting_time/((float)count);
	mean_TA_time=mean_TA_time/((float)count);
	//printf("\n");
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	return 0;
	}

int SJF(FILE *out,struct PCB **processes,int count)
	{
	Non_preemptive(out,processes,count,"SJF");
	return 0;
	}

int Priority(FILE *out,struct PCB **processes,int count)
	{
	Non_preemptive(out,processes,count,"Priority");
	return 0;
	}

int HRRN(FILE *out,struct PCB **processes,int count)
	{
	Non_preemptive(out,processes,count,"HRRN");
	return 0;
	}

int SRTN(FILE *out,struct PCB **processes,int count)
	{
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	fprintf(out,"%s\n","Policy is : SRTN");
	int no_of_processes=0;
	struct node *process_schedule;
	process_schedule=NULL;
	int *pid_array;
	pid_array=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		{
		pid_array[i]=-1;
		}
	int j=0;
	int *completed_pidarray;
	completed_pidarray=(int *)malloc(sizeof(int)*count);
	finish_time=processes[0]->at;
	int last_pid=0;
	for (j=0;j<count;j++)
		{
		if(processes[j]->at<=finish_time)
			{
			pid_array[j]=j;
			last_pid=j;
			}
		}
	while(is_completed(completed_pidarray,count)!=1)
		{
		int min=0;
		min=get_process(pid_array,processes,count,current_time,"rst");
		//printf("min %d\n",min);
		if(min>=0)
			{
			if(last_pid<=count-2 && (processes[min]->rst<=(processes[last_pid+1]->at-current_time)))
				{
				print_process(out,processes[min],&current_time,&waiting_time,&finish_time);
				createnode_at_end(&process_schedule,min);
				mean_waiting_time=mean_waiting_time+waiting_time;
				mean_TA_time=mean_TA_time+(finish_time-processes[min]->at);
				//printf("%s ",processes[min]->pid);
				pid_array[min]=-2;
				completed_pidarray[min]=1;
				processes[min]->rst=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							}
						}
					}
				}
			else 
				{
				if(last_pid<=count-2 && processes[min]->rst>(processes[last_pid+1]->at-current_time))
					{	
					processes[min]->rst=processes[min]->rst-(processes[last_pid+1]->at-current_time);
					current_time=current_time+(processes[last_pid+1]->at-current_time);
					createnode_at_end(&process_schedule,min);
					//printf("%s ",processes[min]->pid);
					//print_process(out,processes[min],&current_time,&waiting_time,&finish_time);
					//fprintf(out,"%s\n",processes[min]->pid);
					pid_array[min]=min;
					}
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							}
						}
					}
				}	
			}
		else 
			{
			current_time=current_time+1;
			for (j=0;j<count;j++)
				{
				if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
					{
					if(processes[j]->at<=current_time)
						{
						pid_array[j]=j;
						last_pid=j;
						}
					}
				}
			}
		if(last_pid==count-1)
			{
			min=get_process(pid_array,processes,count,current_time,"rst");
			print_process(out,processes[min],&current_time,&waiting_time,&finish_time);
			createnode_at_end(&process_schedule,min);
			//printf("%s ",processes[min]->pid);
			mean_waiting_time=mean_waiting_time+waiting_time;
			mean_TA_time=mean_TA_time+(finish_time-processes[min]->at);
			pid_array[min]=-2;
			completed_pidarray[min]=1;
			processes[min]->rst=0;	
			}
		//printf("min %d\n",min);
		//printf("current_time %d\n",current_time);
		}
	mean_waiting_time=mean_waiting_time/((float)count);
	mean_TA_time=mean_TA_time/((float)count);
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	return 0;
	}
