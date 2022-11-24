//
// Created by jia chaobo on 2022/11/24.
//
#include "task.h"
#include "../Scheduler/scheduler.h"
using namespace coroutine;

void Task::AwakeMeFromAwait() {
    mManager_->AwakeTaskByNotify(shared_from_this());
}
