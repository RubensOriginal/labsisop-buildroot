#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_barrier_t barrier;
pthread_mutex_t mutex;
char* buffer;
int counter;
int max_size;


void* write(void* arg) {
    char character = *(char*)arg;

    // printf("Thread %c waiting at the barrier.\n", character);
    // Wait for all threads to reach this point
    pthread_barrier_wait(&barrier);
    
    for (int i = 0; i < 10; i++)
        pthread_mutex_lock(&mutex); 
        printf("%c\n", character);
        pthread_mutex_unlock(&mutex); 
    
    return NULL;
}
int setpriority(pthread_t *thr, int newpolicy, int newpriority)
{
	int policy, ret;
	struct sched_param param;

	if (newpriority > sched_get_priority_max(newpolicy) || newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d\n", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));

		return -1;
	}

	pthread_getschedparam(*thr, &policy, &param);

	param.sched_priority = newpriority;
	ret = pthread_setschedparam(*thr, newpolicy, &param);
	if (ret != 0)
		perror("perror(): ");

	pthread_getschedparam(*thr, &policy, &param);

	return 0;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <tamanho_buffer> <num_threads> <policy>\n", argv[0]);

        return 0;
    }

    int tamanho_buffer = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int policy = atoi(argv[3]);

    pthread_t threads[num_threads];
    char* characters[num_threads];
    int local_buffer[tamanho_buffer];
    buffer = local_buffer;
    max_size = tamanho_buffer;
    counter = 0;

    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    }

    pthread_barrier_init(&barrier, NULL, num_threads);

    for (int i = 0; i < num_threads; i++) {
        characters[i] = 'A' + i;
        pthread_create(&threads[i], NULL, write, &characters[i]);

        int priority = (policy == 1 || policy == 2) ? 1 : 0;
        setpriority(&threads[i], policy, priority);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads ready!\n");

    pthread_barrier_destroy(&barrier);

}