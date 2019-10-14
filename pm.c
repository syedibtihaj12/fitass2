

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "process-visualiser.h"


typedef enum {
    READY, RUNNING, EXIT
}process_state_t;


typedef struct {
    char process_name[11];
    int entryTime;
    int serviceTime;
    int remainingTime;

    process_state_t state;

} pcb_t;
pcb_t queue[10];
pcb_t processes[10];
int front = 0;
int rear = -1;
int count = 0;
_Bool isFull() {
    return count == 10;
}
_Bool isEmpty(){
    return count == 0;
}
void insert(pcb_t process){

    if(!isFull()) {

        if(rear == 9){
            rear = -1;
        }

        queue[++rear] = process;
        count ++;
    }
}
pcb_t removeData(int number){
pcb_t data = processes[front++];

    if(front == number){
        front = 0;

    }

    count --;
    return data;
}
void sort(pcb_t processes[], int number);
void rr(pcb_t processes[], int number);

int main(int argc, char*argv[])
{
    char buffer[100];
    char *filename;




    FILE *fileid = fopen("process.txt", "r");
    if (fileid < 0)
    {
        perror("The file could not be opened");
    }

    fscanf(fileid, "%s %d %d", processes[0].process_name, &processes[0].entryTime, &processes[0].serviceTime);
    processes[0].remainingTime = processes[0].serviceTime;
    int number = 1;
    while (fgets(buffer, sizeof(buffer), fileid) != NULL)
    {

        fscanf(fileid, "%s %d %d", processes[number].process_name, &processes[number].entryTime, &processes[number].serviceTime);
        processes[number].remainingTime = processes[number].serviceTime;
        number = number + 1;

    }
    number = number - 1;

    fclose(fileid);

    sort(processes, number);
    rr(processes, number);

}

void sort(pcb_t processes[], int number)
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

void rr(pcb_t processes[], int number)
{
    int Time = 0;
    insert(processes[0]);
    int wait = 0;
    pcb_t item;
    for (int i =0; i < number; i++)
    {
        Time += processes[i].serviceTime;
        printf("%d\n", Time);

    }
    while(Time > 0)
        {
            item = removeData(number);

            if (item.remainingTime >= 2)
            {
                printf("%s", item.process_name);
                Time -= 2;
                item.state = RUNNING;
                item.remainingTime -= 2;
                sleep(2);
                if (item.state == RUNNING)
                {
                    item.state = READY;
                    insert(item);
                    printf("%d\n", Time);
                    printf("%s", "Change of STATE");
                    printf("%s", "More than the Quantum");

                }

            }

            else if (item.remainingTime >= 0 && item.remainingTime < 2)
            {
                    printf("%s", item.process_name);
                    Time -= 1;
                    sleep(1);
                    item.remainingTime -= 1;
                    item.state = EXIT;
                    printf("%s\n", "EXIT");
                    removeData(number);
                    printf("%d\n", Time);
                    printf("%s", "Less Than the Quantum");

            }

            for (int i = 0; i < number ; i ++)
            {
                if(Time == processes[i+1].entryTime)
                {
                    processes[i+1].state = READY;
                    insert(processes[i]);
                    wait++;
                    printf("%s", "WAIT");
                    printf("%d\n", wait);
                    printf("%s", "ADDING NEW");

                }

            }
         printf("TIME: %d\n", Time);
         printf('Time: %s\n',item.process_name);

        }

}
