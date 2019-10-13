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

int main(int argc, char *argv[])
{	
	
	char buffer[100];//stores file content
	char *filename;//stores filename
	pcb_t processes[10];//array of PCB structure which can store max. 10 processes
	//Assigning filename from which process data has to be read from
	
	//Reading from file and storing data in PCB structure
    FILE *fileid = fopen("process.txt","r");//Opens filename in read mode with a file pointer
	if (fileid < 0)//if fileid is -1,it means the file hasn't been opened
    	{   
        	perror("Cant open file");//write string openError to error stream
        }
   
	fscanf(fileid,"%s %d %d",processes[0].process_name,&processes[0].entryTime,&processes[0].serviceTime);//Reads first process and stores its data in processes array
	//printf("%s",processes[0].process_name);
	//printf("%d",processes[0].entryTime);
	//printf("%d",processes[0].serviceTime);
	int number=1;//To store number of processes
    while(fgets(buffer,sizeof(buffer),fileid) != NULL)//Reading other lines of file ie other processes
	{	
		
		fscanf(fileid,"%s %d %d",processes[number].process_name,&processes[number].entryTime,&processes[number].serviceTime);//storing data of other processes in processes array
		//printf("%s",processes[number].process_name);
		//printf("%d",processes[number].entryTime);
		//printf("%d",processes[number].serviceTime);
		number=number+1;//number of processes incremented
	}
	number=number-1;//last value is garbage value 

	fclose(fileid);//Close file
	sort(processes,number);//function call for sort()
	fcfs(processes,number);//function call for fcfs()

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
void fcfs(pcb_t processes[],int number)
//This function implements first come first serve algorithm for process scheduling
//Arguments:processes array of type pcb_t which has data for all processes sorted according to entry time and number of integer type which has number of processes
//Return value:None
{	
	char *color1 = "black";//background color of interface
	char *color2 = "blue";//foreground color of interface
	initInterface(color1,color2);//initialising the interface for output
	int Time=0;//stores the seconds the processes are being executed
	processes[0].state = READY;//first process will be in READY state by default
	for(int i=0;i<number;i++)
	{
		
		int rowIndex=appendRow(processes[i].process_name); //add row for each process
		appendBlank(rowIndex,processes[i].entryTime);//add blank space from 0 until entry time
		if (rowIndex == 0)//for first process
		{	
			appendBar(rowIndex,processes[0].serviceTime-processes[0].entryTime, color2, processes[0].process_name,0);//add filled bar when process is running
		}
		
		printf("%s</t>",processes[i].process_name);
		printf("%s</n>","HAS ARRIVED");
		int wait = Time-processes[i].entryTime;//waiting time for each process is total time spent on executing on running scheduler subtracted with entry time
		if(wait<0)//if waiting time is less than 0
		{
			wait=0;//set it to 0
		}
		//printf("%d\n",wait);
		if(rowIndex!=0)//for all processes except first one
		{
			appendBar(rowIndex,wait, color2, processes[i].process_name,1);//add dotted bar when process is waiting
		}
		processes[i].state = RUNNING;//assigning process to RUNNING state
		printf("%s</t>",processes[i].process_name);
		printf("%s</n>","IS RUNNING");
		for(int j=0;j<processes[i].serviceTime;j++)//while process is not executed
		{
		
			sleep(1);//program sleeps until process executes
			Time++;//time during which processes execute
			//printf("%d\n",Time);
			
		}
		for(int k=i+1;k<number;k++)//for the next process
			{
			if(Time == processes[k].entryTime)//if total time spent on executing on running scheduler is the entry time of next scheduler
				{
				processes[k].state =  READY;//set it to READY state
				//printf("%d</n>",Time);
				}
			}
		processes[i].state =  EXIT;//exit process once done
		int turn=Time-processes[i].entryTime;//turnaround time is total time spent on executing on running scheduler subtracted with entry time
		if (rowIndex!=0)
		{
			appendBar(rowIndex,(turn-wait), color2, processes[i].process_name,0);//add filled bar when process is running
		}
		printf("%s</t>","Process Name");
		printf("%s</n>",processes[i].process_name);
		printf("%s</t>","Turnaround time");
		printf("%d</n>",turn);
		printf("%s</t>","Waiting time");
		printf("%d</n>",wait);
		printf("%s</n>","Exit");
		
	}
	waitExit();
}




	
	

		
	
   
	
	



