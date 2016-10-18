#include "Helper.h"

int get_process(int *pid_array,struct PCB **processes,int count,int current_time,char string[100])
	{
	int i=0;
	int min=0;
	int flag=0;
	for (i=0;i<count;i++)
		{
		if (pid_array[i]!=-1 && pid_array[i]!=-2)
			{
			min=i;
			if(min==0)
				flag=1;
			break;
			}
		}
	if(string=="SJF")
		{
		int min_rst=processes[min]->rst;
		for(i=0;i<count;i++)
			{
			if(pid_array[i]!=-1 && pid_array[i]!=-2)
				{
				int cur_rst=processes[i]->rst;
				if(cur_rst < min_rst) 
					{
					min = i;
					min_rst=cur_rst;
					if(min==0)
						flag=1;
					}
				}
			}
		}
	if(string=="Priority")
		{
		int min_pr=processes[min]->priority;
		for(i=0;i<count;i++)
			{
			if(pid_array[i]!=-1)
				{
				int cur_pr=processes[pid_array[i]]->priority;
				if(cur_pr < min_pr) 
					{
					min = pid_array[i];
					min_pr=cur_pr;
					if(min==0)
						flag=1;
					}
				}
			}
		}
	if(string=="HRRN")
		{
		float max_rr=(processes[min]->st+(float)(current_time-processes[min]->at))/processes[min]->st;
		//printf("%f\n",max_rr);
		for(i=0;i<count;i++)
			{
			if(pid_array[i]!=-1)
				{
				float cur_rr=(processes[i]->st+(float)(current_time-processes[i]->at))/processes[i]->st;
				if(cur_rr > max_rr) 
					{
					min = pid_array[i];
					max_rr=cur_rr;
					if(min==0)
						flag=1;	
					else
						flag=0;				
					}
				}
			}
		}
	//printf("%d %d\n",min,flag);
	if(min >0)
		{
		return min;
		}
	if(min==0)
		{
		if(flag==1)
			return min;
		return -1;
		}
	//return -1;	
	}

int is_completed(int *completed_array,int count)
	{
	int i=0;
	for(i=0;i<count;i++)
		{
		if (completed_array[i]!=1)
			{
			return 0;
			}
		}
	return 1;
	}
