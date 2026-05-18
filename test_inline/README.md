# Linkage 是什么

Linkage决定：同一个名字在不同翻译单元（`.c`文件）里，是同一个东西，还是各自独立的东西。

## 翻译单元（Translation Unit）

```
源文件 + 它#include的所有头文件 = 一个翻译单元
编译后 = 一个.o文件
```

```
main.c  ──编译──▶  main.o  ┐
other.c ──编译──▶  other.o ├──链接──▶ 可执行文件
util.c  ──编译──▶  util.o  ┘
```

## Internal Linkage

名字只在本翻译单元内可见，其他`.o`看不到它。

```
// foo.h
static inline int add(int a, int b) { return a+b; }
static int counter = 0;  // 每个TU各有一份

main.o  有自己的add()，地址0x1000
other.o 有自己的add()，地址0x2000
        ↑ 互相不知道对方存在，互不干扰
```

## External Linkage

名字全局可见，所有翻译单元共享同一个实体。

```
// foo.c
int global = 42;        // external linkage
void foo() { ... }      // external linkage

main.o   引用global ──┐
other.o  引用global ──┼──▶ 链接器合并 ──▶ 唯一一个global
util.o   引用global ──┘
```

# static inline vs inline in C

## inline only

- 编译器建议内联，但不强制。
- 若函数未被内联（如取了函数地址），需要在某一个`.c`文件中提供`external definition`，否则链接时报`undefined reference`。
- 多个翻译单元（`.c`文件）包含同一头文件时，符号是`external linkage`，链接器需要找到唯一的实体。

## statick inline

- `static`使函数具有`internal linkage`，每个翻译单元有自己的副本。
- 无需额外的`external definition`，不会有链接错误。
- 若编译器不内联，每个`.o`里会有一份私有函数实体，但不会符号冲突。
- 头文件中定义工具函数的惯用写法。

# extern

## 编译器知道声明，但不知道该做什么

当编译器看到`inline int add(int a, int b) { return a + b; }`这个定义时，它会面临一个决策：

- 决策A：在这里（当前`.c`文件）生成`add`函数的机器码（作为函数体）。
- 决策B：不在这里生成机器码，只是记录下“如果需要，可以内联展开这段代码”。

如果没有`extern`，标准行为是倾向于决策A。但这会带来问题：如果`add`函数在`a.c`和`b.c`中都定义了，链接器就会报“多重定义”错误。

加上`extern`后，`extern inline int add(...){...}`的作用是强制编译器选择决策B。它明确告诉编译器：“不要在这里生成这个函数的机器码。它的机器码会在别的`.c`文件里，你到时候去链接它就行。”

## 链接器才是真正需要“外部定义”的角色

编译器根据`extern`指令，没有生成函数体。那么，当程序真的需要调用`add`函数（比如取它的地址，或者编译器决定不内联时），这个函数体从哪里来？
这就是链接器的工作了。链接器需要在所有编译好的`.o`文件里，找到一个名叫`add`的函数体。
所以，你必须在某一个`.c`文件里，提供一个 没有`inline`、没有`static`的普通定义。

## 为什么还是要用extern？

- 对于普通函数：`extern`是多余的。
- 对于`inline`函数：你的直觉是错的。因为`inline`
  的存在，让函数定义有了“生成代码”和“不生成代码”两种可能性。`extern`的作用不是“声明”，而是明确选择“不生成代码”这个选项，从而避免链接时的“多重定义”错误。












