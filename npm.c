//Created by Syed Haque, 27600718 on 29/09/2019.
//Last modified: 13/10/2019.
//This program is demonstrates the First come First Serve Scheduling Algorithm using the ioL system.
//This program reads the data from the "process-data.txt" file input by the user
//It stores the process name, process service time and entry time in the PCB structure.
//With the process data provided, it implements the First Come First Serve Algorithm
//Header files provided with the Questions are needed to implement the code.


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "process-visualiser.h"


typedef enum{
    READY, RUNNING, EXIT //The Three States in which a process can
} process_state_t;

typedef struct {
    char process_name[11];
    int entryTime;
    int serviceTime;
    int remainingTime;

    process_state_t state;

}pcb_t;

int main(int argc, char*argv[]){

    char buffer[100];
    char *name_of_file;
    pcb_t processes[10];


    FILE *fileid = fopen("process.txt", "r");
    if (fileid < 0)
    {
        perror("The file could not be opened");
    }
fscanf(fileid, "%s %d %d", processes[0].process_name, & processes[0].entryTime, &processes[0].serviceTime);



int number = 1;
while (fgets(buffer, sizeof(buffer), fileid) != NULL)
{

    fscanf(fileid, "%s %d %d", processes[number].process_name, & processes[number].entryTime, & processes[number].serviceTime);


    number = number + 1;
}
number = number - 1;

fclose(fileid);
sort(processes,number);
fcfs(processes,number);

}

void fcfs(pcb_t processes[], int number)


{
    char *color_one = "Red";
    char *color_two = "White";
    initInterface(color_one,color_two);
    int Time = 0;
    processes[0].state = READY;
    for(int i =0; i<number; i++)
    {
        int index_of_row = appendRow(processes[i].process_name);
        appendBlank(index_of_row, processes[i].entryTime);
        if(index_of_row == 0)
        {
            appendBar(index_of_row, processes[0].serviceTime-processes[0].entryTime, color_two, processes[0].process_name,0);
        }

        printf("%s</t>", processes[i].process_name);
        printf("%s</n>", "CONGRATS!, HAS ARRIVED");
        int wait = Time-processes[i].entryTime;
        if(wait<0)
        {
            wait = 0;
        }
        if (index_of_row != 0)
        {
            appendBar(index_of_row, wait, color_two, processes[i].process_name, 1);
        }
        processes[i].state = RUNNING;
        printf("%s</t", processes[i].process_name);
        printf("%s</n", "WOHOO!, ITS RUNNING");
        for(int j = 0; j<processes[i].serviceTime; j++)
        {
            sleep(1);
            Time++;
        }
       for (int l = 0; l<number; l++)
       {
            if (Time == processes[l].entryTime)
            {
            processes[l].state = READY;

            }

       }
       processes[i].state = EXIT;
       int turn = Time-processes[i].entryTime;
       if (index_of_row != 0)
       {
       appendBar(index_of_row, (turn-wait), color_two, processes[i].process_name,0);
       }
       printf("%s</t>", "PROCESS NAME");
       printf("%s</n>", processes[i].process_name);
       printf("%s</t>", " TURNAROUND TIME");
       printf("%d</n>", turn);
       printf("%s</t>", "Waiting Time");
       printf("%d</n>", wait);
       printf("%s</n>", "EXIT");

    }


void sort(pcb_t, processes[], int number)
{
    int entry_temp = 0;
    int service_temp = 0;
    char temp [100];
    for (int i = 0; i < number - 1; i ++)
    {
        for(int j = 0; j< number-i-1; j++)
        {

            if(processes[j].entryTime > processes[j+1].entryTime)
            {
                entry_temp = processes[i].entryTime;
                processes[j].entryTime = processes[j+1].entryTime;
                processes[j+1].entryTime = service_temp;

                service_temp = processes[j+1].serviceTime;
                processes[j].serviceTime = processes[j+1].serviceTime;
                processes[j+1].serviceTime = service_temp;

                strcpy(temp, processes[j].process_name);
                strcpy(processes[j].process_name,processes[j+1].process_name);
                strcpy(processes[j+1].process_name, temp);

            }

        }

    }


}
waitExit();

}
