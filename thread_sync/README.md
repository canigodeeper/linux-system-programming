# 线程同步

## 竟争与同步

当多个线程同时访问其所共享的进程资源时，需要相互协调，以防止出现数据不一致、不完整的问题。这就叫线程同步。

## 信号量

信号量时一个计数器，用于控制访问有限共享资源的线程数。

注意：

- 信号量APIs没有生命在`pthread.h`中，而是声明在`semaphore.h`中，失败也不返回错误码，而是返回`-1`，同时设置`errno`。
- 互斥量任何时候都只允许一个线程访问共享资源，而信号量则允许最多value个线程同时访问共享资源，当value为1时，与互斥量等价。

## 条件变量

条件变量可以让调用线程在满足特定条件的情况下暂停。

### pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr)

初始化条件变量，等价写法`pthread_cond_t cond = PTHREAD_COND_INITIALIZER`

### pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)

使调用线程睡入条件变量cond，同时释放互斥锁mutex

- 等待时
    - 内部原子操作：释放mutex，阻塞等待（挂起线程）
- 唤醒时
    - 重新获取锁，如果没有获得锁，会阻塞在这一步
    - 返回

### pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime)

```c
struct timespec {
    time_t tv_sec;   // seconds
    long   tv_nsec;  // nanoseconts [0-999999999]
}
```

### pthread_cond_signal

从条件变量cond中唤醒一个线程，令其重新获得原先的互斥锁

### pthread_cond_broadcast

从条件变量中唤醒所有线程

### 注意：

- 被唤出的线程此刻将从pthread_cond_wait函数返回，但如果该线程无法获得原先的锁，则会继续阻塞在加锁上。
- 当一个线程被从条件变量中唤出以后，导致其睡入条件变量的条件可能还需再判断一次，因其随时有可能被其他线程修改。












