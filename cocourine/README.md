# 协程

过多的线程线程开销巨大。

## 实现协程

线程 + non-blocking I/O

```
sleep(1); // wait_until(T >= cur + 1s);
read(fd, buf, size); // wait_util(fd has data);

# 让编译器帮忙
put_my_self_into_sleep(1); yield();
while(read_sync(fd, buf, size) == EGAIN) {
    yield();
}
```

## goroutine

- 每个CPU上有一个Go Worker Thread（协程调度器）
- 像线程一样使用，像协程一样轻量





















