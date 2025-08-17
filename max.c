#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int max = atoi(argv[2]);
    int n = atoi(argv[1]);

    for (int i = 3; i <= n + 1; i++) {
        int num = atoi(argv[i]);
        if (num > max) {
            max = num;
        }
    }

    printf("%d\n", max);
    return 0;
}
