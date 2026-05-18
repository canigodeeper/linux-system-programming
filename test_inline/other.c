#include "foo.h"

// 为 add_extern 提供 external definition（必须！）
// 去掉这行，链接会报 undefined reference
extern int add_extern(int a, int b);

void other_func() {
    add(1,2);
    printf("=== other.c ===\n");
    printf("add_static addr: %p\n", (void*)add_static);
    printf("add_extern addr: %p\n", (void*)add_extern);
}


