#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Invalid arguments\n");
        return 1;
    }
    int k=atoi(argv[1]);
    for(int i=0;i<k;i++){
        int pid=fork();
        if(pid==0){
            printf("Child Process %d, PID: %d\n", i+1, getpid());
            exit(0);
        }
    }
    return 0;
}