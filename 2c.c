#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int in, out;
    in=dup(STDIN_FILENO);
    out=dup(STDOUT_FILENO);

    int buffer[1024];
    int br=read(in, buffer, sizeof(buffer));
    write(out, "You have entered: ", 18);
    write(out, buffer, br);

    int fd = open("dup.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO);
    
    char str[]="I am re-directed output!";
    write(fd, str, strlen(str));

    close(fd);

    dup2(out, STDOUT_FILENO);
    dup2(in, STDIN_FILENO);
    char str2[]="I am original output!\n";
    write(STDOUT_FILENO, str2, strlen(str2));
    return 0;
}