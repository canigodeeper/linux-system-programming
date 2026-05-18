#include "foo.h"

void other_func();

// gcc -O0 main.c other.c -o test
int main() {
    printf("=== main.c ===\n");
    printf("add_static addr: %p\n", (void*)add_static);
    printf("add_extern addr: %p\n", (void*)add_extern);
    add(1,2);
    add_static(1, 2);
    add_extern(3, 4);

    other_func();
    return 0;
}





