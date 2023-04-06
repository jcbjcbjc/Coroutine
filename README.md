# Coroutine
## Introduction
- c++实现的有栈协程，附加大量上层模块

## examples
- 实现了协程化改造的RPC框架

## Arch
Coroutine 框架的主要模块包括：
- 协程模块
- Reactor模块
- TCP模块
- Pb协议模块
- HTTP 模块
- RPC模块

###协程模块
Coroutine Controller
Coroutine Pool

## Usege
- **单个协程控制**：
```
 Coroutine temp= Coroutine<int>::make_coroutine([]() {
        co_begin(co,int);
        printf("%d\n",co_param(co,0));
        co_yield(co,0);
        co_yield(co,5);
        co_end(co);
    },3);

    temp.invoke();
```
co_begin(co,int) 传入之后要用的对象和lamda表达式的参数类型      
co_param(co,index) 用来获取传入的第index参数      
co_yield(co,x)  用来传递控制权返回调用者等待下一次重入，交换上下文    
co_end 表示协程的结束     
invoke 重入上一次返回的位置，交换上下文切换到协程的上下文
- **托管到Scheduler实现多协程并发**：
```
 Scheduler scheduler(2);

 scheduler.Start();
 
 scheduler.CreateTask<int>(
        [](){int a;},3
 );
```
- 1,创建Scheduler 参数为开启的线程池中的线程数
- 2，另外开启一个线程用来启动Scheduler 调用Start()
- 3,使用提供的CreateTask托管协程，模板参数为lamda参数类型
## Update
- 协程之间的通信机制

