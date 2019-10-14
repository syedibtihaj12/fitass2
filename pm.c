//Created by Syed Haque, 27600718 on 05/10/2019
//Last modified: 14/10/2019.
//This Program demonstrates the Round Robin scheduling algorithm using the ioL system.
//This program reads the data form the process-data.txt file as input by the user.
//The program stores the name of the process, process service time and the process entry time in the PCB Structure provided to us.
//With the process data provided, this program implements the Round Robin Scheduling Algorithm.
//Header files provided with the Questions are needed to implement the code.


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "process-visualiser.h" // Header reference to display the data using ioL.


typedef enum {
    READY, RUNNING, EXIT // The three states in which the process can be in.
}process_state_t;


typedef struct {
    char process_name[11];
    int entryTime;
    int serviceTime;
    int remainingTime;

    process_state_t state;

} pcb_t; // pcb_t Struct definition.

pcb_t queue[10];
pcb_t processes[10]; //This processes of the pcb Structure that can store a max of 10 processes.
int front = 0; //Front variable is set to 0 by default.
int rear = -1; //Rear is set to -1 by default.
int count = 0; // Count variable is set to 0 by default.
_Bool isFull() { //This function checks if we have reached the max capacity of the processes.
    return count == 10;
}
_Bool isEmpty(){ //This function checks if the process count is zero or not.
    return count == 0;
}

pcb_t remove_Data(int number){ //number is passed to the remove data function
pcb_t data = processes[front++];//Increments the front index in the processes array.

    if(front == number){ //Condition to check if the value of front is equal to the number.
        front = 0; //Then front is set to zero.

    }

    count --; //Count is decremented.
    return data; //data is returned after the function execution is completed.
}
void insert(pcb_t process){ // Insert function definition, process is passed to the function.

    if(!isFull()) { //if the process array is not full

        if(rear == 9){
            rear = -1;
        }

        queue[++rear] = process;
        count ++; // Count is incremented.
    }
}


void process_sort(pcb_t processes[], int number); //Process Sort function is called with the process array and the counter passed.
void round_robin(pcb_t processes[], int number); //Round Robin function is called with the process array and the counter passed.

void round_robin(pcb_t processes[], int number) // Function definition for the Round Robin Function.
{
    int Time = 0; // Time is set to zero by default.
    insert(processes[0]); //insert function is executed for the process at index 0.
    int wait = 0; //wait is set to 0 by default.
    pcb_t item; // Item is defined for the structure type PCB.
    for (int i =0; i < number; i++)
    {
        Time += processes[i].serviceTime; //Increment the value of time according the value of service time.
        printf("%d\n", Time); // Prints the total time.

    }
    while(Time > 0) // Checks if the value of Time is lessa than zero
        {
            item = remove_Data(number); // If it is less than zero, then calls remove data function and passes on the number counter.

            if (item.remainingTime >= 2)
            {
                printf("%s", item.process_name); // Prints the item process name.
                Time -= 2; // Time is decremented by 2
                item.state = RUNNING; // The State at this stage is set to RUNNING.
                item.remainingTime -= 2; // The time remaining is decremented by 2.
                sleep(2); // The program sleeps for 2 seconds.
                if (item.state == RUNNING) // If the state of the current process is Running
                {
                    item.state = READY; // THe state is set to Ready.
                    insert(item); // Insert function is called and item is passed.
                    printf("%d\n", Time); // Prints the value of time.
                    printf("%s", "Change of STATE"); // Prints the change of State Message, since the state is changed above.
                    printf("%s", "More than the Quantum"); // Prints this message to tell the user that the running time was more than the quantum time.

                }

            }

            else if (item.remainingTime >= 0 && item.remainingTime < 2) //To check for the condition if the time remaining is between 0 and 2
                                                                        //so it could be 1.
            {
                    printf("%s", item.process_name); //Prints the process name.
                    Time -= 1; //Time is decremented by 1
                    sleep(1); // The process sleeps for one second.
                    item.remainingTime -= 1; // time remaining is decremented by 1.
                    item.state = EXIT; // Process state is set to EXIT.
                    printf("%s\n", "EXIT"); //Exit message is printed on the screen.
                    remove_Data(number); // Remove data function is executed with the number counter passed.
                    printf("%d\n", Time); // The value of the total time is printed.
                    printf("%s", "Less Than the Quantum"); // The Message is printed to let the user know that the running time was less than the Quantum time
                                                            //and that the process has been completed as well, since it was less than Quantum.

            }

            for (int i = 0; i < number ; i ++)
            {
                if(Time == processes[i+1].entryTime) // To check if the Total time is equal to the next process's entry time, then the state of this process would have to be changed.
                {
                    processes[i+1].state = READY; // The state of the process is changed to READY.
                    insert(processes[i]);// the insert function is called on the i th index of the process array, with i being the loop variable.
                    wait++; //Wait is incremented in every iteration of the loop.
                    printf("%s", "WAIT"); // WAit message is printed on the screen.
                    printf("%d\n", wait); // The value of wait counter is printed.
                    printf("%s", "ADDING NEW"); // Adding new message is printed.
                    //The Waiting time is incremented.
                }

            }
         printf("TIME: %d\n", Time); //prints the time.
         printf('Time: %s\n',item.process_name); //prints the process name.

        }

}

int main(int argc, char*argv[]) // Main Program.
{
    char buffer[100]; // This buffer stores the file content which is limited to 100 characters.
    char *filename; // THe pointer stores the filename.

    FILE *fileid = fopen("process.txt", "r"); // The file is opened with read permissions.
    if (fileid < 0) //if the value of the file id is less than zero this means that the file
                    //was not opened successfully, in this case, the error message is displayed.
    {
        perror("The file could not be opened"); //Error Message.
    }

    fscanf(fileid, "%s %d %d", processes[0].process_name, &processes[0].entryTime, &processes[0].serviceTime); // Reads the process at index 0.
    processes[0].remainingTime = processes[0].serviceTime; //The remaining time of the process at index 0 is set to the service of that process.
    int number = 1;
    while (fgets(buffer, sizeof(buffer), fileid) != NULL) // This loop reads other processes.
    {

        fscanf(fileid, "%s %d %d", processes[number].process_name, &processes[number].entryTime, &processes[number].serviceTime);
        //The line above reads the process at index 'number' and saves it in the process array.
        processes[number].remainingTime = processes[number].serviceTime; // The remaining time is set to the service time.
        number = number + 1; //The counter number is incremented.

    }
    number = number - 1;

    fclose(fileid); //FIle is closed after the operation is completed.

    process_sort(processes, number); // Process sort function is called.
    round_robin(processes, number); // ROund Robin function is called.

}

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


