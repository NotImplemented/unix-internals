#include <fenv.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <malloc.h>


struct sigaction existing_handler;


void sighandler()
{
    printf("Caught in handler!\n");
    sigaction(SIGILL, &existing_handler, NULL);
}

int main(void)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sighandler;

    sigaction(SIGILL, &sa, &existing_handler);


    int pagesize = sysconf(_SC_PAGE_SIZE);
    if (pagesize == -1) {
         perror("sysconf");
         exit(1);
    }

    /* allocate aligned page */
    void *ptr = memalign(pagesize, pagesize);

    if (-1 == mprotect(ptr, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        perror("mprotect");
        exit(1);
    }

    ((int *)ptr)[0] = 6;

    ((void(*)())ptr)();


    printf("Done!");
}

