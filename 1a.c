#include<stdio.h>
#include<unistd.h>

int main(){
    int pid=fork();
    if(pid>0){
        printf("P1 PID: %d, PPID: %d\n", getpid(), getppid());
    }
    else if(pid==0){
        printf("P2 PID: %d, PPID: %d\n", getpid(), getppid());
    }
    return 0;
}