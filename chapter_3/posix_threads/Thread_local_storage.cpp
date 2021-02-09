#include <pthread.h>
#include <iostream>

pthread_key_t global_var_key;

void* worker(void* arg) {
    int *p = new int;
    *p = 1;
    pthread_setspecific(global_var_key, p);
    std::cout << global_var_key << std::endl;

    //get the value of the key = 1
    int* global_spec_var = (int*) pthread_getspecific(global_var_key);

    //set global for each thread to two
    *global_spec_var += 1;

    //set global variable to 0
    pthread_setspecific(global_var_key, 0);

    //delete pointer
    delete p;
    pthread_exit(0);
}

int main(void) {
    pthread_t threads[5];

    //used to create a thread specific value for each thread
    pthread_key_create(&global_var_key, 0);

    //create each thread and start them
    for (int i = 0; i < 5; ++i)
        pthread_create(&threads[i], 0, worker, 0);

    //wait for each thread to finish
    for (int i = 0; i < 5; ++i){
        pthread_join(threads[i], 0);
    }
    return 0;
}

