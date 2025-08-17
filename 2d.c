#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h> 
#include<string.h>

int main(){
    int pipefd[2];
    int pid1, pid2;

    pipe(pipefd);

    pid1=fork();
    if(pid1==0){
        char p[16];
        snprintf(p, sizeof(p), "%d", getpid());

        char prompt[] = "Process A (pid: ";
        char prompt_end[] = " ): Input value of x: ";
        
        write(1, prompt, strlen(prompt));
        write(1, p, sizeof(p));
        write(1, prompt_end, strlen(prompt_end));

        char inp[256];
        int br=read(0, inp, sizeof(inp));
        inp[br]='\0';
        int n=atoi(inp);
        n=n*n;

        write(pipefd[1], &n, sizeof(n));

        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    }

    pid2=fork();
    if(pid2==0){
        int n;
        read(pipefd[0], &n, sizeof(n));
        int ans = (int) sqrt((double) n);

        char p[16];
        snprintf(p, sizeof(p), "%d", getpid());

        char result1[] = "Process B (pid: ";
        char result1_end[] = "): Got value from A: ";
        char result2[] = "Process B (pid: ";
        char result2_end[] = "): Original value of x: ";

        write(STDOUT_FILENO, result1, strlen(result1));
        write(STDOUT_FILENO, p, sizeof(p));
        write(STDOUT_FILENO, result1_end, strlen(result1_end));

        char st[256];
        snprintf(st, sizeof(st), "%d", n);
        write(STDOUT_FILENO, st, strlen(st));
        write(1, "\n", 1);

        write(STDOUT_FILENO, result2, strlen(result2));
        write(STDOUT_FILENO, p, sizeof(p));
        write(STDOUT_FILENO, result2_end, strlen(result2_end));
        char str[256];
        snprintf(str, sizeof(str), "%d", ans);

        write(STDOUT_FILENO, str, strlen(str));
        write(1, "\n", 1);

        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
    }
    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}