#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input[50];
    int n;

    while (1) {
        printf(">>> ");

        /*PARSING LOGIC START*/
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) {
            break;  
        }
        else if(strcmp(input, "sum") != 0 && strcmp(input, "max") != 0 && strcmp(input, "min") != 0)
        {
            printf("Command not found\n");
            continue;
        }
        scanf("%d", &n);

        char **args = (char **)malloc((n + 3) * sizeof(char *));
        args[0] = input;
        args[1] = (char *)malloc(10 * sizeof(char));
        sprintf(args[1], "%d", n);

        for (int i = 2; i <= n + 1; i++) {
            args[i] = (char *)malloc(10 * sizeof(char));
            scanf("%s", args[i]);
        }
        args[n + 2] = NULL;
        /*PARSING LOGIC END*/

        int pid = fork();

        if (pid == 0) { 
            if (strcmp(args[0], "sum") == 0 || strcmp(args[0], "min") == 0 || strcmp(args[0], "max") == 0) {
                execv(args[0], args);

                perror("execv failed");
                exit(1);
            }
            exit(0);
        } 
        else { 
            wait(NULL);
        }

        for (int i = 1; i <= n + 1; i++) {
            free(args[i]);
        }
        free(args);
    }

    return 0;
}
