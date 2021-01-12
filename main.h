#include <pthread.h>

#define N_ELEMENTS  10
#define NUM_THREADS 10
#define N   1000000

void *worker(void *arg);