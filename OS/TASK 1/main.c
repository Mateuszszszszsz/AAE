#include <stdio.h>
#include<sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
void child();
void parent();

#define NR_OF_CHILDREN 10
#define ARRAY_SIZE 1000000


int main() {
    printf("Hello, World! We are before fork\n");
    srand(time(NULL));;

    int array[ARRAY_SIZE];
    for(int i = 0 ;i < ARRAY_SIZE;i++)
    {
        array[i] = rand()%100;
    }
    pid_t childrenPIDs[NR_OF_CHILDREN];

    int elementsPerChild = ARRAY_SIZE/NR_OF_CHILDREN;
    for(int i = 0; i <NR_OF_CHILDREN; i++)
    {
        int* miniArrayPtr =&array[elementsPerChild*i];
        childrenPIDs[i] = fork();
        if (childrenPIDs[i] == -1) {
            printf("Error occured while fork\n");
            return (1);
        } else if (childrenPIDs[i] == 0) { //child process
            child(miniArrayPtr,elementsPerChild);
        }

    }
    parent();

}

void child(int* ptr, int n)
{
long int avg = 0;
    for (int i = 0; i< n;i++)
    {
        avg += ptr[i];
    }
avg = avg/n;
   printf("CHILD: Calculating avg value %d\n",avg);
   int exit_status = avg & 0xFF;
   exit(exit_status);
}

void parent()
{
    printf("PARENT: Waiting for child\n");
    long int avg = 0;
            for(int i = 0 ; i< NR_OF_CHILDREN ; i++)
            {
                int wstatus;
                pid_t status = wait(&wstatus);
                printf("PARENT: CHILD EXECUTED and calculated value: %d\n", WEXITSTATUS(wstatus));
                avg +=WEXITSTATUS(wstatus);
            }
            avg = avg/NR_OF_CHILDREN;
    printf("PARENT: PARENT EXECUTED and calculated value: %d\n",avg);
    exit(0);
}
