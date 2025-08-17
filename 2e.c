#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

char *words[5] = {"the", "of", "and", "a", "to"};

void count_words(char *text, size_t length, int *counts) {
    char buffer[1024];
    int buffer_len = 0;

    for (size_t i = 0; i < length; i++) {
        char c = text[i];
        if (isalpha(c) || c == '\'') {
            buffer[buffer_len++] = tolower(c);
            if (buffer_len >= sizeof(buffer) - 1) {
                buffer[buffer_len] = '\0';
                for (int j = 0; j < 5; j++) {
                    if (strcmp(buffer, words[j]) == 0) {
                        counts[j]++;
                    }
                }
                buffer_len = 0;
            }
        } else {
            if (buffer_len > 0) {
                buffer[buffer_len] = '\0';
                for (int j = 0; j < 5; j++) {
                    if (strcmp(buffer, words[j]) == 0) {
                        counts[j]++;
                    }
                }
                buffer_len = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid Arguments\n");
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Number of processes must be positive\n");
        return 1;
    }

    int pipefds[2];
    pipe(pipefds);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int fd = open("bigdata.txt", O_RDONLY);

    int file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    int chunk_size = (file_size) / num_processes;
    int overlap_size = 1024;

    int child_pipes[num_processes][2];
    for (int i = 0; i < num_processes; i++) {
        pipe(child_pipes[i]);
    }

    for (int i = 0; i < num_processes; i++) {
        int pid = fork();

        if (pid == 0) {
            close(pipefds[1]);
            for (int j = 0; j < num_processes; j++) {
                if (j != i) {
                    close(child_pipes[j][0]);
                    close(child_pipes[j][1]);
                }
            }

            off_t start_offset, size;
            if (read(pipefds[0], &start_offset, sizeof(start_offset)) <= 0 ||
                read(pipefds[0], &size, sizeof(size)) <= 0) {
                perror("read");
                exit(1);
            }
            close(pipefds[0]);

            lseek(fd, start_offset, SEEK_SET);
            char *buffer = malloc(size + overlap_size);
            if (!buffer) {
                perror("malloc");
                exit(1);
            }
            ssize_t bytes_read = read(fd, buffer, size + overlap_size);
            if (bytes_read < 0) {
                perror("read");
                free(buffer);
                exit(1);
            }
            buffer[bytes_read] = '\0';

            if (bytes_read >= size) {
                lseek(fd, start_offset + size, SEEK_SET);
                ssize_t extra_bytes = read(fd, buffer + size, overlap_size);
                if (extra_bytes > 0) {
                    buffer[size + extra_bytes] = '\0';
                }
            }

            int counts[5] = {0};
            count_words(buffer, bytes_read, counts);
            free(buffer);

            close(child_pipes[i][0]);
            if (write(child_pipes[i][1], counts, sizeof(counts)) != sizeof(counts)) {
                perror("write");
                exit(1);
            }
            close(child_pipes[i][1]);
            exit(0);
        }
    }

    close(pipefds[0]);
    for (int i = 0; i < num_processes; i++) {
        off_t start_offset = i * chunk_size;
        off_t size = (i == num_processes - 1) ? (file_size - start_offset) : chunk_size;

        write(pipefds[1], &start_offset, sizeof(start_offset));
        write(pipefds[1], &size, sizeof(size));
    }
    close(pipefds[1]);

    int total_counts[5] = {0};
    for (int i = 0; i < num_processes; i++) {
        close(child_pipes[i][1]);
        int counts[5];
        if (read(child_pipes[i][0], counts, sizeof(counts)) == sizeof(counts)) {
            for (int j = 0; j < 5; j++) {
                total_counts[j] += counts[j];
            }
        } else {
            perror("read");
        }
        close(child_pipes[i][0]);
        wait(NULL);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", words[i], total_counts[i]);
    }
    printf("Computation time: %.2fs\n", elapsed);

    close(fd);
    return 0;
}
