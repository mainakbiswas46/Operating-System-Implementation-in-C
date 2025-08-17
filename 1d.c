#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void child(int k) {
    if (k > 0) {
        int pid=fork();
        if (pid == 0) {
            printf("%d: PID: %d PPID: %d created\n",getpid(), getpid(), getppid());
            child(k - 1); 
            printf("%d: PID: %d PPID: %d exited\n",getppid(), getpid(), getppid());
            exit(0);
        } 
        wait(NULL);
        
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid argument.\n");
        return 1;
    }

    int k = atoi(argv[1]);
    if (k <= 0) {
        printf("Please provide a positive number.\n");
        return 1;
    }

    printf("%d: Number of children: %d\n",getpid(), k);
    printf("%d: Main Process’s PID is: %d\n",getpid(), getpid());

    child(k);

    printf("%d: Main Process(PID: %d) exiting\n",getpid(), getpid());

    return 0;
}
