#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include<sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int pid = fork();

    if (pid == 0) {
        int fd;
        char buffer[1024];
        if (argc == 1) { 
            fd = 0;
        } 
        else if (argc == 2) { 
            fd = open(argv[1], O_RDONLY);
        } 
        int br;
        while(br=read(fd, buffer, 1024)){
                write(1, buffer, br);
         }
         if(fd!=0)
         close(fd);

        exit(0);
    } 
    wait(NULL); 

    return 0;
}
