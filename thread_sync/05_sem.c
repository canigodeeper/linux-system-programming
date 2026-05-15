#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

unsigned int g_cn = 0;
sem_t g_sem;

void* thread_add(void* arg) {
    for (unsigned int i = 0; i < 10000000; ++i) {
        sem_wait(&g_sem);
        ++g_cn;
        sem_post(&g_sem);
    }
    return NULL;
}

void* thread_sub(void* arg) {
    for (unsigned int i = 0; i < 10000000; ++i) {
        sem_wait(&g_sem);
        --g_cn;
        sem_post(&g_sem);
    }
    return NULL;
}

int main(void)
{
    sem_init(&g_sem, 0, 1);
    pthread_t tids[2];

    pthread_create(&tids[0], NULL, thread_add, NULL);
    pthread_create(&tids[1], NULL, thread_sub, NULL);
    for (int i = 0; i < 2; ++i) {
        pthread_join(tids[i], NULL);
    }
    printf("g_cn = %u\n", g_cn);

    sem_destroy(&g_sem);

    return 0;
}

