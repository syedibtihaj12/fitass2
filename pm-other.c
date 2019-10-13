// Created by AYESHA ALI,29587778 on 25/09/2019.
// Last modified:8/10/2019
//This program implements the FCFS scheduling algorithm using ioL system.
//It reads process data from "process-data.txt" or a file inputted by user and
//stores process name, process entry and service time in PCB structure
//With the process data it implements FCFS algorithm

//Header files needed to implement the code
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "process-visualiser.h"//Header to use ioL services



typedef enum {
	READY, RUNNING, EXIT//States in which processes exist
} process_state_t ;//instance of enum


typedef struct {
	char process_name[11];
	int entryTime;
	int serviceTime;
	int remainingTime;
	
	process_state_t state;//variable of enum

} pcb_t;//instance of PCB instance
pcb_t queue[10];
pcb_t processes[10];//array of PCB structure which can store max. 10 processes
int front = 0;
int rear = -1;
int count = 0;
_Bool isFull() {
   return count == 10;
}
_Bool isEmpty() {
   return count == 0;
}
void insert(pcb_t process) {
	
   if(!isFull()) {
	
      if(rear == 9) {
         rear = -1;            
      }       

      queue[++rear] = process;
      count++;
   }
}
pcb_t removeData(int number) {
pcb_t data = processes[front++];
	
   if(front == number) {
      front = 0;
   }
	
   count--;
   return data;  
  
}
void sort(pcb_t processes[],int number);
void rr(pcb_t processes[],int number);



int main(int argc, char *argv[])
{	
	
	char buffer[100];//stores file content
	char *filename;//stores filename
	//Assigning filename from which process data has to be read from
	
	//Reading from file and storing data in PCB structure
    FILE *fileid = fopen("process.txt","r");//Opens filename in read mode with a file pointer
	if (fileid < 0)//if fileid is -1,it means the file hasn't been opened
    	{   
        	perror("Cant open file");//write string openError to error stream
        }
   	
   	
	fscanf(fileid,"%s %d %d",processes[0].process_name,&processes[0].entryTime,&processes[0].serviceTime);//Reads first process and stores its data in processes array
	processes[0].remainingTime = processes[0].serviceTime;
	int number=1;//To store number of processes
    while(fgets(buffer,sizeof(buffer),fileid) != NULL)//Reading other lines of file ie other processes
	{	
		
		fscanf(fileid,"%s %d %d",processes[number].process_name,&processes[number].entryTime,&processes[number].serviceTime);
		processes[number].remainingTime = processes[number].serviceTime; //storing data of other processes in processes 
		number=number+1;//number of processes incremented
	}
	number=number-1;//last value is garbage value 
	
	fclose(fileid);//Close file

	//swap();//function call for sort()
	sort(processes,number);
	rr(processes,number);//function call for fcfs()


}
void sort(pcb_t processes[],int number)
//This function sorts the processes array based on acsending order on entry time using bubble sort
//Arguments:processes array of type pcb_t which has data for all processes and number of integer type which has number of processes
//Return value:None
{
	int j=0,i=0;//counters for loops
	int tmpet=0;//temporary variable to sort entry time of processes
	int tmpst=0;//temporary variable to sort service time of processes
	char temp[100];//temporary variable to sort process name of processes
	for (i=0;i<number-1;i++)
	{
		for (j=0;j<number-i-1;j++)
		{
			if (processes[j].entryTime > processes[j+1].entryTime)//if entry time of process is greater than that of the process after
			{
				//Sorting entry time
				tmpet = processes[j].entryTime;
				processes[j].entryTime=processes[j+1].entryTime;
				processes[j+1].entryTime=tmpet;
				//Sorting service time
				tmpst = processes[j+1].serviceTime;
				processes[j].serviceTime=processes[j+1].serviceTime;
				processes[j+1].serviceTime=tmpst;
				//Sorting process name
				strcpy(temp,processes[j].process_name);
				strcpy(processes[j].process_name,processes[j+1].process_name);
				strcpy(processes[j+1].process_name,temp);
			}
		}
	}
	

}

void rr(pcb_t processes[],int number)
{
 	int Time=0;
 	insert(processes[0]);
 	int wait=0;
 	pcb_t item;
 	for(int i=0;i<number;i++)
 	{
 		Time += processes[i].serviceTime;
 		printf("%d\n",Time);
 	}
	while(Time>=0)
	 	
	 	{   
	 		item = removeData(number);
	 	   // printf("Time: %d\n",item.remainingTime);
	 		if(item.remainingTime >= 2)//if remainingtime > qunatum
	 		{   
	 			printf("%s",item.process_name);
	 			Time -= 2;
	 			item.state = RUNNING;
	 			item.remainingTime -= 2;
	 			sleep(2);
	 			if(item.state = RUNNING)
	 			{
	 		 		item.state = READY;
	 		 		insert(item);
	 		 		printf("%d\n",Time);
	 		 		printf("%s","change of statw");
	 		 		printf("%s","more than quantum");
	 			}
	 		}
	 		else if(item.remainingTime >= 0 && item.remainingTime < 2)
	 		{
	 			  
	 				printf("%s",item.process_name);
	 				Time -= 1;
	 				sleep(1);
	 				item.remainingTime -= 1;
	 				item.state = EXIT;
	 				printf("%s\n","exit");
	 				removeData(number);
	 				printf("%d\n",Time);
	 				printf("%s","less than quantum");
	 		}
	 		for(int i=0;i<number;i++)
 			{
			 		if(Time == processes[i+1].entryTime)
			 		{
			 			processes[i+1].state = READY;
			 			insert(processes[i]);
			 			wait++;
			 			printf("%s","wait");
			 			printf("%d\n",wait);
			 			printf("%s","adding new");
			 			//append dotted line here and increment waiting time
			 		}
	 		}
	 		
 		 printf("Time: %d\n", Time);
 		  printf("Time: %s\n", item.process_name);
		}
	}
	
