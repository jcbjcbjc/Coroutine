//
// Created by jia chaobo on 2022/11/24.
//
#include "task.h"
#include "Coroutine/Scheduler/scheduler.h"
using namespace coroutine;

void Task::AwakeMeFromAwait(std::any resumeObj) {
    resumeObject_=resumeObj;
    mManager_->AwakeTaskByNotify(shared_from_this());
}
template<typename... ARGS>
void Task::FillTask(std::function<void()> task,ARGS... args){
    entity_=Coroutine<ARGS...>::make_coroutine(task,args...);
    entity_.SetTask(shared_from_this());
}
