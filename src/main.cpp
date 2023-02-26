#include "Coroutine/coroutine.h"
#include "Coroutine/Scheduler/scheduler.h"
#include "examples/RPC/Common/EventLoop.h"
#include "examples/RPC/RpcClient/RpcClient.h"
using namespace coroutine;

int main() {
    // rsp >= 0x504000;
    // rsp <  0x700000;

    /// normal mode

    Coroutine temp= Coroutine<int>::make_coroutine([]() {
        co_begin(co,int);
        printf("%d\n",co_param(co,0));
        co_yield(co,0);
        co_yield(co,5);
        co_end(co);
    },3);

    temp.invoke();

    /// scheduler mode

    Scheduler scheduler(2);

    Thread t(std::bind(&Scheduler::Start,&scheduler),"1");
    t.start();

    scheduler.CreateTask<int>(
            [](){int a;},3
    );

    scheduler.CreateTask<int,double,float>(
                [](){int a;},1,1,1);

    /// RPC example
    EventLoop loop;
    InetAddress serverAddr("127.0.0.1", 2); // no such server
    RpcClient cli(&loop, serverAddr);
    cli.connect();
    CurrentThread::sleepUsec( 1000);
    {
        Thread t(std::bind(&EventLoop::loop,&loop),"2");
        t.start();
    }


    TaskPtr task=scheduler.CreateEmptyTask();
    task->FillTask<int>([&](){
        int req;
        int res;
        cli.Call(task,req,res);
        int ans=res+1;
        },1);
    scheduler.StartTask(task);


    return 0;
}