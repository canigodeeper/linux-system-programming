#include <stdio.h>
#include <string.h>
#include <pthread.h>

unsigned int g_cn = 0;

void* thread_proc(void* arg) {
    for (unsigned int i = 0; i < 10000000; ++i) {
        ++g_cn;
    }
    return NULL;
}

int main(void) {
    pthread_t tids[2];
    for(int i = 0; i < 2; ++i) {
        pthread_create(&tids[i], NULL, thread_proc, NULL);
    }
    for(int i = 0; i < 2; ++i) {
        pthread_join(tids[i], NULL);
    }
    printf("g_cn = %u\n", g_cn);
    return 0;
}





