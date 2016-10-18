#include "Process_struct.h"

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
