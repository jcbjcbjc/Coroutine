#include "Coroutine/coroutine.h"
#include "Scheduler/scheduler.h"
int main() {
    // rsp >= 0x504000;
    // rsp <  0x700000;
    Scheduler scheduler;
    scheduler.CreateTask<int>(
            [](){int a;},3
            );
    Coroutine::make_coroutine<int>([]() {
        co_begin(co);
        //uint32_t cnt = co_arg(argv, uint32_t);
        co->yield(0);
        co->yield(5);
        //co->yield(cnt);
        //printf("%d\n", cnt);
        //for (int i = 0; i < cnt; i++)
        //co->yield(i);
        co_end(co);
    },3);

    temp.invoke();

    //temp.param(0, 3);
    //COROUTINE_FOREACH(temp, i) { printf("%d\n", i); }
    return 0;
}