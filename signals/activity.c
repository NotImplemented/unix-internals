#include <time.h>
#include <stdio.h>
#include <fenv.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <malloc.h>



long long counter = 0;
FILE *fd;

void CreatePidFile() {
    FILE* f = fopen("pidfile", "w");
    if (!f) {
        return;
    }
    fprintf(f, "%d\n", (int)getpid());
    fclose(f);
}

void PrintLine(FILE* f) {
    time_t ltime;
    struct tm result;
    char stime[32];

    ltime = time(NULL);
    localtime_r(&ltime, &result);
    strftime(stime, sizeof(stime), "%F %T", &result);
    fprintf(f, "[%s] PID: %d, Counter: %lld\n", stime, (int)getpid(), counter);
    ++counter;
}


struct sigaction existing_handler;


void sighandler()
{
    printf("Caught SIGHUP in handler!\n");

    fclose(fd);

    fd = fopen("log.txt", "a");
    if (!fd) {
        fprintf(stderr, "unable to open file\n");
    }

    //sigaction(SIGHUP, &existing_handler, NULL);
}


int RunMain() {

    CreatePidFile();

    /* Add signal handler */
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sighandler;

    sigaction(SIGHUP, &sa, &existing_handler);

    fd = fopen("log.txt", "a");
    if (!fd) {
        fprintf(stderr, "unable to open file\n");
        return 1;
    }
    while (1) {
        PrintLine(fd);
        fflush(fd);
        usleep(10000);
    }
    fclose(fd);
    return 0;
}


int main(int argc, char* argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        return RunMain();
    } else {
        fprintf(stderr, "Started a daemon with PID = %d.\n", pid);
        return 0;
    }
    return 0;
}
