#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

pthread_mutex_t lock_a, lock_b;
atomic_int allowed = 0;


void* first(void* arguments)
{
    printf("First thread started.\n");
    pthread_mutex_lock(&lock_a);

    printf("First thread lock_a acquired.\n");

    while(!allowed);

    printf("First thread continues.\n");

    pthread_mutex_lock(&lock_b);

    pthread_mutex_unlock(&lock_b);

    pthread_mutex_unlock(&lock_a);
    printf("First thread finished.\n");
}


void* second(void* arguments)
{
    printf("Second thread started.\n");
    pthread_mutex_lock(&lock_b);

    printf("Second thread lock_b acquired.\n");

    while(!allowed);

    printf("Second thread continues.\n");


    pthread_mutex_lock(&lock_a);

    pthread_mutex_unlock(&lock_a);


    pthread_mutex_unlock(&lock_b);
    printf("Second thread finished.\n");
}



int main(void) 
{
    int result_code = 0;
    pthread_t thread_first, thread_second;

    printf("Creating first thread.\n");
    result_code = pthread_create(&thread_first, NULL, &first, 0);
    assert(!result_code);

    printf("Creating second thread.\n");
    result_code = pthread_create(&thread_second, NULL, &second, 0);
    assert(!result_code);

    printf("Threads are created.\n");

    while(getchar() != EOF);

    ++allowed;

    printf("Threads are allowed to continue.\n");

    result_code = pthread_join(thread_first, NULL);
    assert(!result_code);

    result_code = pthread_join(thread_second, NULL);
    assert(!result_code);


    printf("Main program finished.\n");
  
    return 0;
}
