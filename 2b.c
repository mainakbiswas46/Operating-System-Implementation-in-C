#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid Arguments\n");
        return 1;
    }

    close(0);
    int src_fd = open(argv[1], O_RDONLY);
    
    close(1);
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    char buffer[1024];
    int br;

    while ((br = read(0, buffer, sizeof(buffer)))) {
        write(1, buffer, br);
    }
    close(src_fd);
    close(dest_fd);

    return 0;
}
