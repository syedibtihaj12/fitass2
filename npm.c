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
#include "process-visualiser.h" //Header reference to display the data using ioL.


typedef enum{
    READY, RUNNING, EXIT //The Three States in which a process can be in
} process_state_t;

typedef struct {
    char process_name[11];
    int entryTime;
    int serviceTime;
    int remainingTime;
    int pos;
    process_state_t state;

}pcb_t; //pcb_t Struct definition.

void process_sort(pcb_t processes[], int number)
// This function sorts the array containing the processes according to the entry time using
// bubble sorting algorithm.

{
    int entry_temp = 0; // Temporary variable to sort the entry time of the process
    int service_temp = 0; // Temporary variable to sort the service time of the process.
    char temp [100]; // Temporary variable for sorting the process name of processes.
    for (int i = 0; i < number - 1; i ++)
    {
        for(int j = 0; j< number-i-1; j++)
        {

            if(processes[j].entryTime > processes[j+1].entryTime)
            {
                //Sorting the entry time
                entry_temp = processes[i].entryTime;
                processes[j].entryTime = processes[j+1].entryTime;
                processes[j+1].entryTime = service_temp;

                //Sorting the Service time.
                service_temp = processes[j+1].serviceTime;
                processes[j].serviceTime = processes[j+1].serviceTime;
                processes[j+1].serviceTime = service_temp;

                ///The Process name is being sorted below.
                strcpy(temp, processes[j].process_name);
                strcpy(processes[j].process_name,processes[j+1].process_name);
                strcpy(processes[j+1].process_name, temp);

            }

        }

    }


}

void FCFS(pcb_t processes[], int number)
//This function is implementing the First Come First Serve Scheduling Algorithm.


{
    char *color_one = "black"; // This is the background color of the interface.
    char *color_two = "white"; //This is the color of the bars.
    initInterface(color_one,color_two); //Interface is being initialised.
    int Time = 0; //This variable stores the seconds that are taken by the processes when they are executed.
    processes[0].state = READY; // This sets the process on index one to be in the READY state by default.
    for(int i =0; i<number; i++)
    {
        int index_of_row = appendRow(processes[i].process_name); //This adds a new row for each process.
        appendBlank(index_of_row, processes[i].entryTime); // This adds a blank space starting from zero until the entry time.
        if(index_of_row == 0) //Condition check for the first process.
        {
            appendBar(index_of_row, processes[0].serviceTime-processes[0].entryTime, color_two, processes[0].process_name,0);
        }

        printf("%s</t>", processes[i].process_name); // Prints the value in process name.
        printf("%s</n>", "CONGRATS!, HAS ARRIVED"); // Prints the message.
        int wait = Time-processes[i].entryTime; // waiting time for every process is the difference between total time spent while execution and the entry time.
        if(wait<0) //Condition to check if the waiting time is less than zero.
        {
            wait = 0; // Waiting time is then set to zero.
        }
        if (index_of_row != 0) // Condition to check for all other processes except the first one.
        {
            appendBar(index_of_row, wait, color_two, processes[i].process_name, 1);// This adds a dotted bar to the display while the process is waiting.
        }
        processes[i].state = RUNNING;// The Process is assigned to the RUNNING State.
        printf("%s</t", processes[i].process_name);
        printf("%s</n", "WOHOO!, ITS RUNNING");
        for(int j = 0; j<processes[i].serviceTime; j++) //This loop runs while the program is not executed.
        {
            sleep(1); // The program goes to sleep until the process is executing.
            Time++; // Time is incremented.
        }
       for (int l = 0; l<number; l++) // For the next upcoming process.
       {
            if (Time == processes[l].entryTime) //This is to check if the total time on the running scheduler is equal to the entry time of the next scheduler
            {
            processes[l].state = READY; // This sets the process to the ready state.

            }

       }
       processes[i].state = EXIT; //This sets the EXIT state to the process when it's done execution.
       int turnaround_time = Time-processes[i].entryTime;
       if (index_of_row != 0)
       {
       appendBar(index_of_row, (turnaround_time-wait), color_two, processes[i].process_name,0); // This adds a solid bar on the screen to demonstrate that the process is running.
       }
       printf("%s</t>", "PROCESS NAME"); // Prints the process name
       printf("%s</n>", processes[i].process_name); // Prints the value in the process name.
       printf("%s</t>", " TURNAROUND TIME"); //Prints the Turnaround time.
       printf("%d</n>", turnaround_time); // Prints the value of TurnAround Time.
       printf("%s</t>", "Waiting Time");// Prints the Waiting Time.
       printf("%d</n>", wait); //Prints the value of the waiting time.
       printf("%s</n>", "EXIT"); // Prints Exit Message.

    }

waitExit();

}

int main(int argc, char*argv[]){

    char buffer[100]; // This buffer stores the file content.
    char *name_of_file; //This pointer stores the filename that is being passed in the argument.
    pcb_t processes[10]; //This is array of the PCB structure which can store a maximum of 10 processes.

    if (argc==2) //If the Argument counter is 2
    {
        name_of_file = argv[1]; //name of file is set to the file name in the index 1 of the arguments array.

    }
    else //otherwise
    {
        name_of_file = "process-data.txt"; // This file is assigned the file name in the program as a default case.
    }
    //The code segment below reads fromthe file and stores the data in the PCB Structure.
    FILE *fileid = fopen("process.txt", "r"); //Opens the file in read mode and passes the reference to the file pointer.
    if (fileid < 0) // If the value at the filer pointer is less than zero. this means that the program was unable to open the file.
    {
        perror("The file could not be opened"); // If the file could not be opened then this message will be displayed.
    }
    fscanf(fileid, "%s %d %d", processes[0].process_name, & processes[0].entryTime, &processes[0].serviceTime);
    //The line above reads the process at index 0 and saves it in the process array.
    int number = 1; //This number is the counter of the number of processes.
    while (fgets(buffer, sizeof(buffer), fileid) != NULL) //This loop reads other processes.
    {

    fscanf(fileid, "%s %d %d", processes[number].process_name, & processes[number].entryTime, & processes[number].serviceTime);
    //The above line reads the processes according to their indexes and then saves them in the process array.
    number = number + 1; //The counter is incremented when a new process is read and saved in the process array.
    processes[number].pos = number;
    }
    number = number - 1;
    fclose(fileid); //This Statement closes the file once the read operation has been completed.

    process_sort(processes,number); //FUnction call for the function process Sort.

    FCFS(processes,number);// Function call for the function First Come First Serve
                            //which demonstrates the scheduling algorithm.

}



