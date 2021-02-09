#include <pthread.h>
#include <stdlib.h>
#include <iostream>

#define NUM_THREADS 5

//The PTHREAD_MUTEX_INITIALIZER macro initializes the static mutex mutex, 
//setting its attributes to default values. This macro should only be used for 
//static mutexes, as no error checking is performed.
static pthread_mutex_t func_mutex = PTHREAD_MUTEX_INITIALIZER;

void func() {
    pthread_mutex_lock(&func_mutex);

    std::cout << "should be self contained" << std::endl;

    pthread_mutex_unlock(&func_mutex);
}

void * worker(void* arg) {
    int value = *((int *) arg);
    func();
    // add more logic
}

int main(int argc, char** argv) {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int result_code;

    for(unsigned int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        result_code = pthread_create(&threads[i], 0, worker, (void*) &thread_args[i]);
    }
    for(int i = 0; i < NUM_THREADS; ++i) {
        result_code = pthread_join(threads[i], 0);
    }

    exit(0);
}
