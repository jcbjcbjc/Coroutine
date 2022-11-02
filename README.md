# Coroutine
## Introduction
c++的有栈实现，实现了单个协程最基本的控制操作，以及提供了Scheduler进行协程调度和管理可以多协程进行。使用时可以自己控制协程也可以托管到Scheduler
## Usege
- 单个协程控制：
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
co_yield(co,x)  用来传递
