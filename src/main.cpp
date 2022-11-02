#include "Coroutine/coroutine.h"
#include "Scheduler/scheduler.h"
int main() {
    // rsp >= 0x504000;
    // rsp <  0x700000;

    //CurrentThread::tid();

    //Scheduler scheduler(2);
   /* scheduler.CreateTask<int>(
            [](){int a;},3
            );*/
    //scheduler.Start();



    Coroutine temp= Coroutine<int>::make_coroutine([]() {
        co_begin(co,int);
        printf("%d\n",co_param(co,0));
        co->yield(0);
        co->yield(5);

        //printf("%d\n",std::get<0>(co->param()));

        co_end(co);
    },3);

    temp.invoke();

    //temp.param(0, 3);
    //COROUTINE_FOREACH(temp, i) { printf("%d\n", i); }
    return 0;
}