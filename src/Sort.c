#include "Sort.h"

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