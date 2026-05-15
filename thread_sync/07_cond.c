#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_STOCK 20 // 最大容量
char g_storage[MAX_STOCK];
size_t g_stock = 0; // 当前容量
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t g_not_empty = PTHREAD_COND_INITIALIZER;

// 显示库存
void show(const char* who, const char* op, char product) {
    printf("%s:", who);
    size_t i;
    for (i = 0; i < g_stock; ++i) {
        printf("%c", g_storage[i]);
    }
    printf("%s%c\n", op, product);
}

// 生产者线程
void* producer(void* arg) {
    const char* who = (char*)arg;
    for (;;) {
        char product = 'A' + rand() % 26;
        pthread_mutex_lock(&g_mutex);
        if (g_stock >= MAX_STOCK)
        {
            printf("\033[;;32m%s：满仓！\033[0m\n", who);
            pthread_cond_wait(&g_not_full, &g_mutex);
        }
        show(who, "<-", product); // show里面有共享资源
        g_storage[g_stock++] = product;
        pthread_cond_signal(&g_not_empty);
        pthread_mutex_unlock(&g_mutex);

        usleep((rand() % 100) * 1000);
    }
    return NULL;
}

// 消费者线程
void* customer(void* arg) {
    const char* who = (char*)arg;
    for (;;) {
        pthread_mutex_lock(&g_mutex);
        if (!g_stock)
        {
            printf("\033[;;31m%s：空仓！\033[0m\n", who);
            pthread_cond_wait(&g_not_empty, &g_mutex);
        }
        char product = g_storage[--g_stock];
        show(who, "->", product);
        pthread_cond_signal(&g_not_full);
        pthread_mutex_unlock(&g_mutex);

        usleep((rand() % 100) * 1000);
    }
    return NULL;
}

int main(void) {
    srand(time(NULL));

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid1, tid2;
    pthread_create(&tid1, &attr, producer, "生产者");
    pthread_create(&tid2, &attr, customer, "消费者");


    getchar();

    return 0;
}


