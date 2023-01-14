#include "Coroutine/coroutine.h"
#include "Coroutine/Scheduler/scheduler.h"
using namespace coroutine;

int main() {
    // rsp >= 0x504000;
    // rsp <  0x700000;

    Coroutine temp= Coroutine<int>::make_coroutine([]() {
        co_begin(co,int);
        printf("%d\n",co_param(co,0));
        co_yield(co,0);
        co_yield(co,5);
        co_end(co);
    },3);

    temp.invoke();


    Scheduler scheduler(2);

    scheduler.Start();
    scheduler.CreateTask<int>(
            [](){int a;},3
    );


    scheduler.CreateTask<int,double,float>(
                [](){int a;},1,1,1);

    return 0;
}