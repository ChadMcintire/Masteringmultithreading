#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

const int COUNT_TRIGGER = 10;
const int COUNT_LIMIT = 12;

int count = 0;
int threads_ids[3] = {0,1,2};

pthread_mutex_t count_mutex;
pthread_cond_t count_cv;

void* add_count(void* t) {
    int tid = (long) t;
    for (int i = 0; i < COUNT_TRIGGER; ++i) {
    pthread_mutex_lock(&count_mutex);
    count++;
    if (count == COUNT_LIMIT) {
        //call unblocks at least one of the threads that are blocked on the specified condition variable cond
        pthread_cond_signal(&count_cv);
    }

    pthread_mutex_unlock(&count_mutex);
    sleep(1);
    }

    pthread_exit(0);
}

void* watch_count(void* t) {
    int tid = (long) t;

    pthread_mutex_lock(&count_mutex);
    if (count < COUNT_LIMIT) {
        pthread_cond_wait(&count_cv, &count_mutex);
    }

    pthread_mutex_unlock(&count_mutex);
    pthread_exit(0);
}

int main (int argc, char* argv[]) {
    long tid1 = 1, tid2 = 2, tid3 =3;
    pthread_t threads[3];
    pthread_attr_t attr;

      
    //initialize mutex and conditional variable
    pthread_mutex_init(&count_mutex,0);
    pthread_cond_init(&count_cv, 0);

    pthread_attr_init(&attr);

    //PTHREAD_CREATE_JOINABLE is the default state of the attr object state
    //int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    //void *(*start_routine) (void *), void *arg);
    pthread_create(&threads[0], &attr, watch_count, (void *) tid1);
    pthread_create(&threads[1], &attr, add_count, (void *) tid2);
    pthread_create(&threads[2], &attr, add_count, (void *) tid3);

    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], 0);
    }

    //good practice to destroy thread objects
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_cv);
    return 0;
}
