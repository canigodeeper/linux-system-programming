#ifndef FOO_H
#define FOO_H

#include <stdio.h>

// 方式1: static inline
static inline int add_static(int a, int b) {
    printf("add_static called, addr=%p\n", (void*)add_static);
    return a + b;
}

// 方式2: 纯 inline (external linkage)
inline int add_extern(int a, int b) {
    printf("add_extern called, addr=%p\n", (void*)add_extern);
    return a + b;
}

#endif


