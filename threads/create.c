#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
 
volatile int g_counter = 0;
 
#define STACK_SIZE 4096


volatile int count = 0;

void* ThreadFuncIncrement(void* arg) {
    for (int i = 0; i < 1024; ++i) {
        ++count;
    }
    return NULL;
}

int ThreadFunc(void* ptr) {
    volatile int* shouldTerminate = ptr;
    while (!*shouldTerminate) {
        printf("[thread] = %d\n", g_counter);
        sleep(1);
    }
    printf("[thread] Terminating...\n");
    return 0;
}
 
int main() {
    printf("My pid: %d\n", getpid());
 
    void* stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("Unable to allocate thread stack");
        return 1;
    }
 
    int shouldTerminate = 0;
 
    int flag = SIGCHLD | CLONE_SIGHAND | CLONE_FS | CLONE_VM | CLONE_FILES;
    pid_t tid = clone(ThreadFunc, stack + STACK_SIZE, flag, &shouldTerminate);
    if (tid == -1) {
        perror("clone()");
        return 1;
    }
 
    printf("Done! Thread pid: %d\n", tid);
 
    while (1) {
        if (getchar() == EOF) {
            shouldTerminate = 1;
            break;
        }
        g_counter++;
    }
 
    printf("Waiting for thread...\n");
    int status;
    if (waitpid(tid, &status, __WALL) == -1) {
        perror("waitpid()");
    }
 
    free(stack);
    printf("Done! Graceful shutdown.\n");
    return 0;
}
