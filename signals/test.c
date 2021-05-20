#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define N (10 * 1024 * 1024)

int a[N] = {1, 2, 3};

int main() {
    int s = 0;
    for (int i = 0; i < N; ++i) {
        s += a[i];
    }
    printf("%d\n", s);
 
    memset(a, 0, sizeof(int) * N);
    
    while(1);

    return 0;
}
