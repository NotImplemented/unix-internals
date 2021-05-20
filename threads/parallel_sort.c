#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>


struct thread_parameters
{
    int i, j;
    int thread_number;
    int* array;
};


int compare(const void* a_ptr, const void* b_ptr)
{
    int a = *(const int*)a_ptr;
    int b = *(const int*)b_ptr;
 
    return a - b; // fails if INT_MIN is present
}


void *perform_work(void *arguments)
{
    struct thread_parameters parameters = *((struct thread_parameters *)arguments);
    
    int thread_idx = parameters.thread_number;
    int i = parameters.i, j = parameters.j;
    int* array = parameters.array;
    int chunk_size = j - i;

    printf("Thread %d: Started. Parameters = %d, %d\n", thread_idx, i, j);
  
    qsort(array + i, chunk_size, sizeof(int), compare);

    printf("Thread %d: Ended.\n", thread_idx);
}


int* create_array(const int N)
{
    int idx = 0;
    int* result = malloc(N * sizeof(int));
    assert(result != 0);

    for(;idx < N; ++idx)
    {
	result[idx] = random() % 0xFFFF;
    }

    return result;
}


int verify_array(int array[], const int N)
{
    int idx = 0;
    for(;idx+1 < N; ++idx)
    {
	if (array[idx] > array[idx+1])
            return 0;
    }

    return 1;
}


int main(int argc, char* argv[]) 
{
    printf("Main has started.\n");

    const int N = 256 * 1024 * 1024;
    const int num_threads = atoi(argv[1]);
    int chunk_size = N / num_threads;

    printf("Total threads = %d, chunk size = %d\n", num_threads, chunk_size);

    pthread_t threads[num_threads];
    struct thread_parameters parameters[num_threads];
    int indexes[num_threads];

    int idx;
    int result_code;

    int *data = create_array(N);

    printf("Main: preparing thread data.\n");
    for(idx = 0; idx < num_threads; ++idx)
    {
       parameters[idx].i = idx * chunk_size;
       parameters[idx].j = (idx + 1) * chunk_size;
       parameters[idx].thread_number = idx;
       parameters[idx].array = data;
    }
  
    //create all threads one by one
    for (idx = 0; idx < num_threads; idx++) 
    {
        printf("Main: Creating thread %d.\n", idx);
        result_code = pthread_create(&threads[idx], NULL, perform_work, &parameters[idx]);
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

    printf("Main: merging chunks\n");

    int *result = malloc(N * sizeof(int));

    for (idx = 0; idx < num_threads; idx++) 
    {
       indexes[idx] = 0;
    }

    for(int step = 0; step < N; ++step) 
    {
        int smallest_idx = -1, smallest_value;
        for(int idx = 0; idx < num_threads; ++idx)
        {
            if (indexes[idx] == chunk_size)
               continue;

            int value = data[chunk_size * idx + indexes[idx]];
            if (smallest_idx == -1 || smallest_value > value)
            {
                smallest_idx = idx;
                smallest_value = value;
            }
        }

        result[step] = smallest_value;
        indexes[smallest_idx]++;
    }

    int ok = verify_array(result, N);

    if (ok)
       printf("Array is sorted!\n");
    else
       printf("ERROR!\n");

    free(result);
    free(data);

    printf("Main has ended.\n");
  
    return 0;
}
