#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define DINERS 5

typedef enum tag_State
{
    THINKING,
    HUNGRY,
    EATING,
} ESTAT;

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t g_conds[DINERS] = {
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
};

const char* g_names[DINERS] = {
    "张三",
    "李四",
    "王五",
    "赵六",
    "钱七",
};

ESTAT g_stats[DINERS] = {
    THINKING,
    THINKING,
    THINKING,
    THINKING,
    THINKING,
};

void eat(int i)
{
    int right = (i + 1) % DINERS;
    int left = (i - 1 + DINERS) % DINERS;
    pthread_mutex_lock(&g_mutex);
    while (g_stats[left] == EATING || g_stats[right] == EATING) { // 左或右有人在吃
        g_stats[i] = HUNGRY;
        printf("%s：快点吧，饿死我了～～～\n", g_names[i]);
        pthread_cond_wait(&g_conds[i], &g_mutex);
    }
    g_stats[left] = EATING;
    printf("%s：终于可以吃一口了！\n", g_names[i]);
    pthread_mutex_unlock(&g_mutex);
    usleep(rand() % 100 * 10000);
}

void think(int i)
{
    int right = (i + 1) % DINERS;
    int left = (i - 1 + DINERS) % DINERS;
    pthread_mutex_lock(&g_mutex);
    g_stats[i] = THINKING;
    printf("%s：吃饱了，开始思考...\n", g_names[i]);
    pthread_cond_signal(&g_conds[left]);
    pthread_cond_signal(&g_conds[right]);
    pthread_mutex_unlock(&g_mutex);
    usleep(rand() % 100 * 10000);
}

void* dinner(void* arg)
{
    int i = (int)arg;
    for (;;)
    {
        eat(i);
        think(i);
    }
    return NULL;
}


int main(void)
{
    srand(time(NULL));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t tid;
    for (size_t i = 0; i < DINERS; ++i)
    {
        pthread_create(&tid, &attr, dinner, (void*)i);
    }

    getchar();

    return 0;
}



