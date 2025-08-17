#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc<2) {
        printf("Executable file not found\n");
        return 1;
    }
    printf("1f PID: %d\n",getpid());
    int pid = fork();

    if (pid == 0) {
        char *path = argv[1];
        char *args[] = { argv[1], NULL };

        execv(path, args);

        printf("Executable file not found\n");
        return 1;
    } 
    wait(NULL);

    return 0;
}

