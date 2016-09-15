

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <bsd/stdlib.h>

struct node
    {
    int value;
    struct node *next;
    };

struct PCB
    {
    char pid[100];
    int at;
    int st;
	int rst;
	int wt;
    int priority;
    };

int createnode_at_end(struct node **n,int data)
    {
    struct node *newnode;
    struct node *last;
    newnode=(struct node *)malloc(sizeof(struct node));
    newnode->value=data;
    newnode->next=NULL;
    last=*n;
    if(*n==NULL)
        {
        *n=newnode;
        return 0;
        }
    while(last->next!=NULL)
        {
        last=last->next;
        }
    last->next=newnode;
    return 0;
    }

struct node *enqueue(struct node *n,int *pid)
    {
    struct node *temp;
    temp=(struct node*)malloc(sizeof(struct node));
	*pid=-1;
    if(n==NULL)
        {
        //printf("Under flow\n");
		*pid=-1;
        return NULL;
        }
    *pid=n->value;
    temp=n->next;
    return temp;
    }

int print(struct node *n)
    {
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp=n;
    //printf("yes\n");
    while(temp!=NULL)
        {
        printf("%d\n",temp->value);
        temp=temp->next;
        }
    return 0;
    }

int queue(struct node **n,int data)
    {
    createnode_at_end(n,data);
    }

// Process-id AT ST Pr(if any) FT TAT mean-TAT Ini-WT Inter-WT TWT mean-WT

int print_process_schedule(FILE *out,struct node *process_schedule,struct PCB **processes)
	{
	struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp=process_schedule;
    //printf("yes\n");
    while(temp!=NULL)
        {
        fprintf(out,"%s ",processes[temp->value]->pid);
        temp=temp->next;
        }
	fprintf(out,"%s","\n");
    return 0;
	}

int FCFS(FILE *out,struct PCB **processes,int count)
	{
	//FILE *out;
	int i=0;
	//int j=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	//out = fopen("out.dat","w+");
	fprintf(out,"%s\n","Policy is : FCFS");
	struct node *process_schedule;
	process_schedule=NULL;
	for(i=0;i<count;i++)
		{
		//printf("%s","Hi");
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
		//fprintf(out,"\n");
		//printf("%s ",processes[i]->pid);
		createnode_at_end(&process_schedule,i);
		}
	//printf("\n");
	mean_waiting_time=mean_waiting_time/((float)count);
	mean_TA_time=mean_TA_time/((float)count);
	//printf("Policy is : FCFS\n");
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	return 0;
	}

void sort(struct PCB **processes,int count)
    {
    int i=0,j=0;
    struct PCB *temp;
    for(i=0;i<count-1;i++)
        {
        for(j=0;j<count-i-1;j++)
            {
            if(processes[j]->at > processes[j+1]->at)
                {
                temp=processes[j];
                processes[j]=processes[j+1];
                processes[j+1]=temp;
                }
            }
        }
    }

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

int Non_preemptive(FILE *out,struct PCB **processes,int count,char string[100])
	{
    //FILE *out;
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	//out = fopen("out.dat","w+");
	fprintf(out,"Policy is : %s\n",string);
	//printf("Process Schedule : \n");
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
	//FILE *out;
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	//out = fopen("out.dat","w+");
	fprintf(out,"%s\n","Policy is : SRTN");
	//printf("Process Scedule : \n");
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
	//printf("\n");
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	return 0;
	}

int RR(FILE *out,struct PCB **processes,int count,int time_quantum)
	{
	//FILE *out;
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	//out = fopen("out.dat","w+");
	fprintf(out,"%s\n","Policy is RR");
	//printf("Process Schedule : \n");
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
				//printf("i %d\n",i);
				if(completed_pidarray[i]!=1 && pid_array[i]!=-2 && pid_array[i] < 0)
					{
					//printf("Hi1564\n");
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
		//}
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
	//printf("\n");
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	return 0;
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
    //printf("yes\n");
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
		}
	//print(Queues[1]);
	//printf("Hi");
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
	//out = fopen("out.dat","w+");
	fprintf(out,"%s\n","Policy is MLQ");
	//printf("Process Schedule : \n");
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
	//printf("%s\n",Q_name[0]);
	//if(strcmp(Q_name[0],"RR")==0)	{
		//printf("RR4154\n");
	//}	
	//print(Queues[0]);
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
					//printf("i %d\n",i);
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
			//printf("%d \n",i);
			//print(Queues[i]);
			//printf("pl\n");
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
					//printf("i %d\n",i);
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
			//printf("%d \n",i);
			//print(Queues[i]);
			//printf("pl\n");
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
	//printf("\n");
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	}

void MLFQ(FILE *out,struct PCB **processes,int count)
	{
	//FILE *out;
	int i=0;
	int finish_time=0;
	int waiting_time=0;
	int current_time=0;
	float mean_waiting_time=0;
	float mean_TA_time=0;
	int no_of_queues=0;
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
		}
	//print(Queues[1]);
	//printf("Hi");
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
	//out = fopen("out.dat","w+");
	fprintf(out,"%s","Policy is MLFQ\n");
	//printf("Process Schedule : \n");
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
	//printf("%s\n",Q_name[0]);
	//if(strcmp(Q_name[0],"RR")==0)	{
		//printf("RR4154\n");
	//}	
	//print(Queues[0]);
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
					//printf("i %d\n",i);
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
			//printf("%d \n",i);
			//print(Queues[i]);
			//printf("pl\n");
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
					//printf("i %d\n",i);
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
			//printf("%d \n",i);
			//print(Queues[i]);
			//printf("pl\n");
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
	//printf("\n");
	fprintf(out,"%s","Process Schedule : ");
	print_process_schedule(out,process_schedule,processes);
	fprintf(out,"Mean Waiting Time : %f\n",mean_waiting_time);
	fprintf(out,"Mean Turn Around Time : %f\n",mean_TA_time);
	}

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
