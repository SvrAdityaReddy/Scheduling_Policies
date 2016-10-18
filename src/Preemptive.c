#include "Preemptive.h"
#include "Helper.h"

int RR(FILE *out,struct PCB **processes,int count,int time_quantum)
	{
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	fprintf(out,"%s\n","Policy is RR");
	finish_time=processes[0]->at;
	struct node *process_schedule;
	process_schedule=NULL;
	int *pid_array;
	pid_array=(int *)malloc(sizeof(int)*count);
	struct node *n;
        n=NULL;
	for(i=0;i<count;i++)
		{
		pid_array[i]=-1;
		}
	int last_pid=0;
	for (i=0;i<count;i++)
		{
		if(processes[i]->at<=finish_time)
			{
			createnode_at_end(&n,i);
			pid_array[i]=i;
			last_pid=i;
			}
		}
	//print(n);
	int *completed_pidarray;
	completed_pidarray=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		{
		completed_pidarray[i]=0;
		}
	int pid=0;
	while(is_completed(completed_pidarray,count)!=1)
		{
		n=enqueue(n,&pid);
		if(pid>=0 && (processes[pid]->rst<=time_quantum))
			{
			print_process(out,processes[pid],&current_time,&waiting_time,&finish_time);
			//printf("%s ",processes[pid]->pid);
			createnode_at_end(&process_schedule,pid);
			mean_waiting_time=mean_waiting_time+waiting_time;
			mean_TA_time=mean_TA_time+(finish_time-processes[pid]->at);
			pid_array[pid]=-2;
			completed_pidarray[pid]=1;
			processes[pid]->rst=0;
			//printf("current_time %d\n",current_time);
			for (i=0;i<count;i++)
				{
				if(completed_pidarray[i]!=1 && pid_array[i]!=-2 && pid_array[i] < 0)
					{
					if(processes[i]->at<=current_time)
						{
						pid_array[i]=i;
						last_pid=i;
						createnode_at_end(&n,i);
						}
					}
				}
			}
		else 
			{
			if(pid>=0 && processes[pid]->rst>time_quantum)
				{	
				//printf("%s ",processes[pid]->pid);
				createnode_at_end(&process_schedule,pid);
				processes[pid]->rst=processes[pid]->rst-time_quantum;
				current_time=current_time+time_quantum;
				pid_array[pid]=pid;
				//printf("current_time %d\n",current_time);
				}
			for (i=0;i<count;i++)
				{
				if(completed_pidarray[i]!=1 && pid_array[i]!=-2 && pid_array[i] < 0)
					{
					if(processes[i]->at<=current_time)
						{
						pid_array[i]=i;
						last_pid=i;
						createnode_at_end(&n,i);
						}
					}
				}
			createnode_at_end(&n,pid);
			}
		if(pid<0)
			{
			current_time=current_time+1;
			for (i=0;i<count;i++)
				{
				if(completed_pidarray[i]!=1 && pid_array[i]!=-2 && pid_array[i] < 0)
					{
					if(processes[i]->at<=current_time)
						{
						pid_array[i]=i;
						last_pid=i;
						createnode_at_end(&n,i);
						}
					}
				}
			}
		}
	mean_waiting_time=mean_waiting_time/((float)count);
	mean_TA_time=mean_TA_time/((float)count);
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	return 0;
	}

int get_process_preemptive(struct node *Queue,struct PCB **processes,int current_time,char string[100])
	{
	int min=-1;
	if(Queue!=NULL)
		{
		min=(Queue)->value;	
		}
	else
		return -1;
	struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp=Queue;
	if(strcmp(string,"RST")==0)
		{
		while(temp!=NULL)
        	{
			if(processes[temp->value]->rst<processes[min]->rst)
				{
				min=temp->value;
				}
        	//printf("%d\n",temp->value);
        	temp=temp->next;
        	}
		}
	if(strcmp(string,"Priority")==0)
		{
		while(temp!=NULL)
        	{
			if(processes[temp->value]->priority<processes[min]->priority)
				{
				min=temp->value;
				}
        	//printf("%d\n",temp->value);
        	temp=temp->next;
        	}
		}
	if(strcmp(string,"SJF")==0)
		{
		while(temp!=NULL)
        	{
			if(processes[temp->value]->rst<processes[min]->rst)
				{
				min=temp->value;
				}
        	//printf("%d\n",temp->value);
        	temp=temp->next;
        	}
		}
	if(strcmp(string,"HRRN")==0)
		{
		float max_rr=(processes[min]->rst+(float)(current_time-processes[min]->at))/processes[min]->rst;
		while(temp!=NULL)
        	{
			float cur_rr=(processes[temp->value]->rst+(float)(current_time-processes[temp->value]->at))/processes[temp->value]->rst;
			if(cur_rr > max_rr)
				{
				min=temp->value;
				}
        	//printf("%d\n",temp->value);
        	temp=temp->next;
        	}
		}	
	return min;
	//return -1;
	}

void delete_key(struct node **head_ref,int key)
	{
	// Store head node
    struct node* temp = *head_ref, *prev;
 
    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->value == key)
    {
        *head_ref = temp->next;   // Changed head
        free(temp);               // free old head
        return;
    }
 
    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->value != key)
    {
        prev = temp;
        temp = temp->next;
    }
 
    // If key was not present in linked list
    if (temp == NULL) return;
 
    // Unlink the node from linked list
    prev->next = temp->next;
 
    free(temp);  // Free memory
	}

int Q_Assignment(struct PCB **processes,int pid,int no_of_queues)
	{
	if(processes[pid]->rst <=2)
		{
		return 0;
		}
	if(processes[pid]->rst > 2 && processes[pid]->rst <=5)
		{
		return 1;
		}
	no_of_queues=no_of_queues-1;
	//return (( rand() % no_of_queues ));
	return 2;
	}

void MLQ(FILE *out,struct PCB **processes,int count)
	{
	//FILE *out;
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	int no_of_queues=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	fprintf(out,"%s\n","Policy is MLQ");
	printf("Policy is MLQ\n");
	printf("Enter no_of_queues: ");
	scanf("%d",&no_of_queues);
	//printf("%d\n",no_of_queues);
	fprintf(out,"Number of Queues : %d\n",no_of_queues);
	struct node *process_schedule;
	process_schedule=NULL;
	int *Q_tq;
	Q_tq=(int *)malloc(sizeof(int)*no_of_queues);
	char **Q_name;
	Q_name=(char **)malloc(sizeof(char *)*no_of_queues);
	print_schedulingpolicies();
	struct node **Queues;
	Queues=(struct node **)malloc(sizeof(struct node *)*no_of_queues);
	for(i=0;i<no_of_queues;i++)
		{
		Queues[i]=NULL;
		//Queues[i]=(struct node *)malloc(sizeof(struct node));
		printf("Enter Scheduling policy for level %d Q:",i+1);
		Q_name[i]=(char *)malloc(sizeof(char)*100);
		//printf("%s\n",Q_name[i]);
		scanf("%s",Q_name[i]);
		//printf("%s\n",Q_name[i]);
		printf("Enter time quantum:");
		scanf("%d",&Q_tq[i]);
		fprintf(out,"Q_level %d : %s : time_quantum = %d\n",i+1,Q_name[i],Q_tq[i]);
		}
	//print(Queues[1]);
	int *pid_array;
	pid_array=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		{
		pid_array[i]=-1;
		}
	int *completed_pidarray;
	completed_pidarray=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		{
		completed_pidarray[i]=0;
		}
	finish_time=processes[0]->at;
	int last_pid=0;
	for (i=0;i<count;i++)
		{
		if(processes[i]->at<=finish_time)
			{
			createnode_at_end(&Queues[Q_Assignment(processes,i,no_of_queues)],i);
			pid_array[i]=i;
			last_pid=i;
			}
		}
	int pid=0;
	while(is_completed(completed_pidarray,count)!=1)
		{
		for(i=0;i<no_of_queues;i++)
			{
			if((strcmp(Q_name[i],"RR")==0) || strcmp(Q_name[i],"FCFS")==0 )	{
			Queues[i]=enqueue(Queues[i],&pid);
			//printf("current_time %d\n",current_time);
			if(pid>=0 && (processes[pid]->rst<=Q_tq[i]))
				{
				print_process(out,processes[pid],&current_time,&waiting_time,&finish_time);
				createnode_at_end(&process_schedule,pid);
				//printf("%s ",processes[pid]->pid);
				mean_waiting_time=mean_waiting_time+waiting_time;
			    mean_TA_time=mean_TA_time+(finish_time-processes[pid]->at);
				pid_array[pid]=-2;
				completed_pidarray[pid]=1;
				processes[pid]->rst=0;
				//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[Q_Assignment(processes,j,no_of_queues)],j);
							//createnode_at_end(&Queues[i],j);
							}
						}
					}
				//print(Queues[0]);
				}
			else 
				{
				if(pid>=0 && processes[pid]->rst>Q_tq[i])
					{	
					//printf("%s ",processes[pid]->pid);
					createnode_at_end(&process_schedule,pid);
					processes[pid]->rst=processes[pid]->rst-Q_tq[i];
					current_time=current_time+Q_tq[i];
					pid_array[pid]=pid;
					//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[Q_Assignment(processes,j,no_of_queues)],j);
							//createnode_at_end(&Queues[i],j);
							}
						}
					}
				if(i+1!=no_of_queues)
					createnode_at_end(&Queues[i],pid);
				else
					createnode_at_end(&Queues[i],pid);
					}
				}
			//print(Queues[i]);
			//printf("current_time %d\n",current_time);
			if(pid>=0)
				{
				i=-1;
				}
			//i=-1;
			}
			else
				{
				//int pid=-1;
				pid=get_process_preemptive(Queues[i],processes,current_time,Q_name[i]);
				if(pid>=0 && (processes[pid]->rst<=Q_tq[i]))
				{
				delete_key(&Queues[i],pid);
				print_process(out,processes[pid],&current_time,&waiting_time,&finish_time);
				createnode_at_end(&process_schedule,pid);
				//printf("%s ",processes[pid]->pid);
				mean_waiting_time=mean_waiting_time+waiting_time;
			    mean_TA_time=mean_TA_time+(finish_time-processes[pid]->at);
				pid_array[pid]=-2;
				completed_pidarray[pid]=1;
				processes[pid]->rst=0;
				//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[Q_Assignment(processes,j,no_of_queues)],j);
							//createnode_at_end(&Queues[i],j);
							}
						}
					}
				//print(Queues[0]);
				}
			else 
				{
				if(pid>=0 && processes[pid]->rst>Q_tq[i])
					{
					delete_key(&Queues[i],pid);	
					processes[pid]->rst=processes[pid]->rst-Q_tq[i];
					createnode_at_end(&process_schedule,pid);
					//printf("%s ",processes[pid]->pid);
					current_time=current_time+Q_tq[i];
					pid_array[pid]=pid;
					//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						//printf("Hi1564\n");
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[Q_Assignment(processes,j,no_of_queues)],j);
							//createnode_at_end(&Queues[i],j);
							}
						}
					}
				if(i+1!=no_of_queues)
					createnode_at_end(&Queues[i],pid);
				else
					createnode_at_end(&Queues[i],pid);
					}
				}
			//print(Queues[i]);
			//printf("current_time %d\n",current_time);
			if(pid>=0)
				{
				i=-1;
				}
			//i=-1;
			}
		}
		int j=0;
		if(pid<0)
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
							createnode_at_end(&Queues[Q_Assignment(processes,j,no_of_queues)],j);
							//createnode_at_end(&Queues[i],j);
							}
						}
					}
				}
		//printf("current_time %d\n",current_time);

		}
	mean_waiting_time=mean_waiting_time/((float)count);
	mean_TA_time=mean_TA_time/((float)count);
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	}

void MLFQ(FILE *out,struct PCB **processes,int count)
	{
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	int no_of_queues=0;
	printf("Policy is MLFQ\n");
	printf("Enter no_of_queues: ");
	scanf("%d",&no_of_queues);
	//printf("%d\n",no_of_queues);
	struct node *process_schedule;
	process_schedule=NULL;
	int *Q_tq;
	Q_tq=(int *)malloc(sizeof(int)*no_of_queues);
	char **Q_name;
	Q_name=(char **)malloc(sizeof(char *)*no_of_queues);
	print_schedulingpolicies();
	struct node **Queues;
	Queues=(struct node **)malloc(sizeof(struct node *)*no_of_queues);
	fprintf(out,"%s","Policy is MLFQ\n");	
	fprintf(out,"Number of Queues : %d\n",no_of_queues);
	for(i=0;i<no_of_queues;i++)
		{
		Queues[i]=NULL;
		//Queues[i]=(struct node *)malloc(sizeof(struct node));
		printf("Enter Scheduling policy for level %d Q:",i+1);
		Q_name[i]=(char *)malloc(sizeof(char)*100);
		//printf("%s\n",Q_name[i]);
		scanf("%s",Q_name[i]);
		//printf("%s\n",Q_name[i]);
		printf("Enter time quantum:");
		scanf("%d",&Q_tq[i]);
		fprintf(out,"Q_level %d : %s : time_quantum = %d\n",i+1,Q_name[i],Q_tq[i]);
		}
	//print(Queues[1]);
	int *pid_array;
	pid_array=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		{
		pid_array[i]=-1;
		}
	int *completed_pidarray;
	completed_pidarray=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		{
		completed_pidarray[i]=0;
		}
	finish_time=processes[0]->at;
	int last_pid=0;
	for (i=0;i<count;i++)
		{
		if(processes[i]->at<=finish_time)
			{
			createnode_at_end(&Queues[0],i);
			pid_array[i]=i;
			last_pid=i;
			}
		}
	int pid=0;
	while(is_completed(completed_pidarray,count)!=1)
		{
		for(i=0;i<no_of_queues;i++)
			{
			if((strcmp(Q_name[i],"RR")==0) || strcmp(Q_name[i],"FCFS")==0 )	{
			Queues[i]=enqueue(Queues[i],&pid);
			//printf("current_time %d\n",current_time);
			if(pid>=0 && (processes[pid]->rst<=Q_tq[i]))
				{
				print_process(out,processes[pid],&current_time,&waiting_time,&finish_time);
				//printf("%s ",processes[pid]->pid);
				createnode_at_end(&process_schedule,pid);
				mean_waiting_time=mean_waiting_time+waiting_time;
			    mean_TA_time=mean_TA_time+(finish_time-processes[pid]->at);
				pid_array[pid]=-2;
				completed_pidarray[pid]=1;
				processes[pid]->rst=0;
				//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[0],j);
							}
						}
					}
				//print(Queues[0]);
				}
			else 
				{
				if(pid>=0 && processes[pid]->rst>Q_tq[i])
					{	
					processes[pid]->rst=processes[pid]->rst-Q_tq[i];
					createnode_at_end(&process_schedule,pid);
					//printf("%s ",processes[pid]->pid);
					current_time=current_time+Q_tq[i];
					pid_array[pid]=pid;
					//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						//printf("Hi1564\n");
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[0],j);
							}
						}
					}
				if(i+1!=no_of_queues)
					createnode_at_end(&Queues[i+1],pid);
				else
					createnode_at_end(&Queues[i],pid);
					}
				}
			//printf("current_time %d\n",current_time);
			if(pid>=0)
				{
				i=-1;
				}
			//i=-1;
			}
			else
				{
				//int pid=-1;
				pid=get_process_preemptive(Queues[i],processes,current_time,Q_name[i]);
				if(pid>=0 && (processes[pid]->rst<=Q_tq[i]))
				{
				delete_key(&Queues[i],pid);
				print_process(out,processes[pid],&current_time,&waiting_time,&finish_time);
				createnode_at_end(&process_schedule,pid);
				//printf("%s ",processes[pid]->pid);
				mean_waiting_time=mean_waiting_time+waiting_time;
			    mean_TA_time=mean_TA_time+(finish_time-processes[pid]->at);
				pid_array[pid]=-2;
				completed_pidarray[pid]=1;
				processes[pid]->rst=0;
				//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[0],j);
							}
						}
					}
				//print(Queues[0]);
				}
			else 
				{
				if(pid>=0 && processes[pid]->rst>Q_tq[i])
					{
					delete_key(&Queues[i],pid);	
					processes[pid]->rst=processes[pid]->rst-Q_tq[i];
					createnode_at_end(&process_schedule,pid);
					//printf("%s ",processes[pid]->pid);
					current_time=current_time+Q_tq[i];
					pid_array[pid]=pid;
					//printf("current_time %d\n",current_time);
				int j=0;
				for (j=0;j<count;j++)
					{
					if(completed_pidarray[j]!=1 && pid_array[j]!=-2 && pid_array[j] < 0)
						{
						if(processes[j]->at<=current_time)
							{
							pid_array[j]=j;
							last_pid=j;
							createnode_at_end(&Queues[0],j);
							}
						}
					}
				if(i+1!=no_of_queues)
					createnode_at_end(&Queues[i+1],pid);
				else
					createnode_at_end(&Queues[i],pid);
					}
				}
			//printf("current_time %d\n",current_time);
			if(pid>=0)
				{
				i=-1;
				}
			//i=-1;
			}
		}
		int j=0;
		if(pid<0)
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
							createnode_at_end(&Queues[0],j);
							}
						}
					}
				}
		//printf("current_time %d\n",current_time);

		}
	mean_waiting_time=mean_waiting_time/((float)count);
	mean_TA_time=mean_TA_time/((float)count);
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	}