#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>


int compare(const void* a_ptr, const void* b_ptr)
{
    int a = *(const int*)a_ptr;
    int b = *(const int*)b_ptr;
 
    return a - b; // fails if INT_MIN is present
}


void *perform_work(void *arguments)
{
    thread_parameters parameters = *((thread_parameters *)arguments);
    
    int thread_index = parameters.thread_number;
    int i = parameters.i, j = parameters.j;
    int* array = parameters.array;

    printf("Thread %d: Started. Parameters = %d, %d\n", thread_index, i, j);
  
    qsort(array, j - i, sizeof(int), compare);

    printf("Thread %d: Ended.\n", thread_index);
}

struct thread_parameters
{
    int i, j;
    int thread_number;
    int* array;
};

int* create_array(const int N)
{
    int idx = 0;
    int* result = malloc(N);
    for(;idx < N; ++idx)
    {
	result[idx] = random();
    }

    return result;
}

int main(int argc, char* argv[]) {

    const int N = 256 * 1024 * 1024;
    const int num_threads = atoi(argv[1]);
    int chunk_size = N / num_threads;

    pthread_t threads[num_threads];
    thread_parameters[num_threads];
    int indexes[num_threads];

    int idx;
    int result_code;

    int *data = create_array(N);

    for(idx = 0; idx < num_threads; ++idx)
    {
       thread_parameters[idx].i = idx * chunk_size;
       thread_parameters[idx].j = (idx + 1) * chunk_size;
       thread_parameters[idx].thread_number = idx;
       thread_parameters[idx].array = data;
    }

  
    //create all threads one by one
    for (idx = 0; idx < num_threads; idx++) 
    {
        printf("Main: Creating thread %d.\n", idx);
        thread_args[idx] = idx;
        result_code = pthread_create(&threads[idx], NULL, perform_work, &thread_args[idx]);
        assert(!result_code);
    }

    printf("Main: All threads are created.\n");

    //wait for each thread to complete
    for (idx = 0; idx < num_threads; idx++) 
    {
        result_code = pthread_join(threads[idx], NULL);
        assert(!result_code);
        printf("Main: Thread %d has ended.\n", idx);
    }

    printf("Main: merging chunks");

  int *result = malloc(N);

  for (idx = 0; idx < num_threads; idx++) 
  {
     indexes[idx] = 0;
  }

  for(int steps = 0; steps < N; ++steps) 
  {
     int smallest_idx = -1;
     for(int idx = 0; idx < num_threads; ++i)
     {
        if (indexes[idx] == N)
           continue;

        int value = data[chunk_size * idx + indexes[idx]];

        if (smallest_idx == -1 || smallest_value > value)
        {
           smallest_idx = idx;
           smalles_value = value;
        }
     }
     result[step] = value;
     indexes[smallest_idx]++;
  }


  printf("Main program has ended.\n");
  return 0;
}
