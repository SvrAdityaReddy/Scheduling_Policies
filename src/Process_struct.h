#ifndef _PROCESS_STRUCT_H
#define _PROCESS_STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int createnode_at_end(struct node **n,int data);
struct node *enqueue(struct node *n,int *pid);
int print(struct node *n);
int queue(struct node **n,int data);

#endif