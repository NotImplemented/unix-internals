#include <stdio.h>
#include <unistd.h>

int main() {
    write(1, "a", 1);
    fork();
}
