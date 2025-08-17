#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid arguments.\n");
        return 1;
    }

    int k = atoi(argv[1]);
    if(k<=0){
    printf("Please provide a positive number.\n")
    return 1;
    }
    printf("%d: Parent process, PID: %d\n", getpid(), getpid());

    for (int i = 0; i < k; i++) {
        int pid = fork();
        if (pid == 0) {
            printf("%d: Child process %d, PID: %d, PPID: %d\n", getpid(), i + 1, getpid(), getppid());
            sleep(5);
            exit(0); 
        }
    }

    for (int i = 0; i < k; i++) {
        int pid = wait(NULL);
        if (pid > 0) {
            printf("%d: PID: %d exited\n", getpid(), pid);
        }
    }

    return 0;
}
