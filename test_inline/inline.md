# inline

## 基本想法

在头文件声明，在一个.c文件中实现。方案完全可行，但会失去内联效果。

```c
// utils.h
inline int add(int a, int b);  // 仅仅声明

// utils.c
#include "utils.h"
inline int add(int a, int b) {  // 定义在这里
    return a + b;
}

// main.c
#include "utils.h"
int main() {
    return add(1, 2);  // 调用
}
```

### 结果

这段代码可以编译链接通过，但是`add`函数不会被内联。

### 原因

当编译器编译main.c时，它只看到了`inline int add(int a, int b);`这个声明，看不到函数体`{ return a + b; }`。
不知道函数体是什么，编译器就无法在调用点展开它。最终main.c会生成一个对`add`的普通函数调用，链接时去`utils.c`中找到定义。

## 内联的核心要求：编译器必须看到函数体

`inline`的本质是编译时优化，不是链接时优化。它的工作方式是：

- 编译器编译main.c时，读到`add(1, 2)`
- 编译器需要决定：是生成`call add`指令，还是直接用`mov $3, %rax`替换
- 要做出这个决定，编译器必须在此时此刻知道`add`的代码是什么

因此，`inline`函数的定义（函数体）必须在调用它的`.c`文件中可见。标准做法就是把定义放在头文件里，让所有包含它的`.c`文件都能看到。

## static inline 就是为解决这个问题而生的

如果直接把定义放在头文件，又会导致链接时多重定义。所以C语言提供了`static inline`：

```c
// utils.h
static inline int add(int a, int b) {
    return a + b;
}
```

- 每个`.c`文件：都看到了完整的函数体，可以进行内联优化。
- `static`关键字：告诉编译器"这个函数是当前文件私有的"，即使没有内联，每个`.c`文件也会生成自己独立的函数副本，不会在链接时冲突。

























