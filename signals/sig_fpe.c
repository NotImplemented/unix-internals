#include <fenv.h>
#include <signal.h>
#include <stdio.h>

struct sigaction existing_handler;


void sighandler()
{
    printf("Caught in handler!\n");
    sigaction(SIGFPE, &existing_handler, NULL);
}

int main(void)
{
    int value = 256, divisor = 0;

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; 
    sa.sa_sigaction = sighandler;
    
    sigaction(SIGFPE, &sa, &existing_handler);

    printf("Dividing by zero...\n");
    value /= divisor;

    printf("Done!");
}
