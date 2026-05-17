# python协程

## python协程锁的本质

协程锁的本质是一个状态标志 + 等待队列，它利用事件循环的调度机制来实现异步阻塞（而不是线程阻塞）。

与线程锁的区别：

- 线程锁：获取不到锁时，操作系统挂起整个线程（内核态切换）
- 协程锁：获取不到锁时，当前协程让出控制权（yield），事件循环调度其他协程运行

## 真实 asyncio.Lock 的核心实现

```python
# 简化自CPython的asyncio/locks.py
class Lock:
    def __init__(self):
        self._locked = False
        self._waiters = collections.deque()

    async def acquire(self):
        if not self._locked and not self._waiters:
            self._locked = True
            return True

        # 创建Future作为等待凭证
        fut = self._loop.create_future()
        self._waiters.append(fut)

        try:
            await fut  # 关键：这里让出CPU
        finally:
            # 被唤醒后，从队列移除（也可能被cancel）
            self._waiters.remove(fut)

        self._locked = True
        return True

    def release(self):
        if self._locked:
            self._locked = False
            # 唤醒下一个等待者
            if self._waiters:
                self._waiters.popleft().set_result(True)
```

## 关键机制解析

### Future 对象作为"暂停令牌"

```python
fut = self._loop.create_future()  # 创建未完成的Future
await fut  # await未完成的Future 会立即让出CPU
```

await future的行为：

- 如果future已完成（有结果/异常），立即继续执行
- 如果future未完成，保存当前协程的上下文，暂停执行，让出CPU

### 事件循环的作用

- 协程A获取锁成功 → 执行临界区
- 协程B获取锁失败 → await future → 保存B的状态 → 切换到事件循环
- 事件循环调度其他就绪协程（比如协程C）
- 协程A释放锁 → future.set_result() → B被标记为就绪 → 下次循环调度B

### 等待队列的作用

```python
self._waiters = deque()  # 双端队列
self._waiters.append(future)  # 排队
self._waiters.popleft().set_result(True)  # 唤醒最先等待的
```

保证公平性：先等待的协程先获得锁（FIFO）

## 关键理解：await fut 之前，锁已经被占用了

这几行代码只在锁已经被别人占用时才会执行，不是拿到锁的情况！

```python
async def acquire(self):
    if not self._locked and not self._waiters:
        # 情况1：锁空闲 → 直接拿到锁，返回True
        self._locked = True
        return True
    
    # else情况
    # 情况2：锁被占用 → 进入这里（你问的这几行）
    fut = self._loop.create_future()
    self._waiters.append(fut)

    try:
        await fut  # 在这里暂停，让出CPU
    finally:
        self._waiters.remove(fut)

    # 被唤醒后，锁现在是我的了
    self._locked = True
    return True
```

### 完整流程演示

```python
# 3个协程同时竞争同一个锁

# 协程A：第一个执行acquire()
# → _locked=False, _waiters=[] → 直接拿到锁，返回True
# → 执行临界区代码...

# 协程B：第二个执行acquire()
# → _locked=True（被A占着）→ 进入else分支
# → 创建fut_B，加入队列 [_waiters = [fut_B]]
# → await fut_B  # 暂停！让出CPU

# 协程C：第三个执行acquire()
# → _locked=True → 进入else分支
# → 创建fut_C，加入队列 [_waiters = [fut_B, fut_C]]
# → await fut_C  # 暂停！让出CPU

# 此时：协程A在运行，B和C都在等待

# 协程A：释放锁
def release(self):
    self._locked = False
    if self._waiters:
        fut = self._waiters.popleft()  # 取出fut_B
        fut.set_result(None)  # 唤醒协程B！

# 协程B：被唤醒后，从await fut_B处继续执行
# → finally: self._waiters.remove(fut_B)  # 从队列移除自己
# → self._locked = True  # 拿到锁
# → return True
```



















































































































































