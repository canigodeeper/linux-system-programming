#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t g_mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_mutex_b = PTHREAD_MUTEX_INITIALIZER;

void* thread_proc1(void* arg)
{
    pthread_t tid = pthread_self();

    printf("%lu线程：等待A...\n", tid);
    pthread_mutex_lock(&g_mutex_a);
    printf("%lu线程：获得A！\n", tid);

    sleep(1);

    printf("%lu线程：等待B...\n", tid);
    pthread_mutex_lock(&g_mutex_b);
    printf("%lu线程：获得B！\n", tid);

    pthread_mutex_unlock(&g_mutex_b);
    pthread_mutex_unlock(&g_mutex_a);
}

void* thread_proc2(void* arg)
{
    pthread_t tid = pthread_self();

    printf("%lu线程：等待B...\n", tid);
    pthread_mutex_lock(&g_mutex_b);
    printf("%lu线程：获得B！\n", tid);

    sleep(1);

    printf("%lu线程：等待A...\n", tid);
    pthread_mutex_lock(&g_mutex_b);
    printf("%lu线程：获得A！\n", tid);

    pthread_mutex_unlock(&g_mutex_a);
    pthread_mutex_unlock(&g_mutex_b);
}


int main(void)
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread_proc1, NULL);
    pthread_create(&tid2, NULL, thread_proc2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&g_mutex_a);
    pthread_mutex_destroy(&g_mutex_b);

    return 0;
}