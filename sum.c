#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int sum = 0;
    int n = atoi(argv[1]);

    for (int i = 2; i <= n + 1; i++) {
        sum += atoi(argv[i]);
    }

    printf("%d\n", sum);
    return 0;
}
