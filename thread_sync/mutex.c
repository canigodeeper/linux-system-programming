#include <stdio.h>
#include <string.h>
#include <pthread.h>

unsigned int g_cn = 0;
pthread_mutex_t g_mutex;

void* thread_proc(void* arg) {
    for (unsigned int i = 0; i < 10000000; ++i) {
        pthread_mutex_lock(&g_mutex);
        ++g_cn;
        pthread_mutex_unlock(&g_mutex); // 释放锁，操作系统随机唤醒一个线程
    }
    return NULL;
}


int main(void)
{
    pthread_mutex_init(&g_mutex, NULL);
    pthread_t tids[2];
    for (int i = 0; i < 2; ++i) {
        pthread_create(&tids[i], NULL, thread_proc, NULL);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_join(tids[i], NULL);
    }
    printf("g_cn = %u\n", g_cn);
    pthread_mutex_destroy(&g_mutex);

    return 0;
}
