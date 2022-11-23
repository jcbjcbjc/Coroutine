//
// Created by jcbjcbjc on 11/23/22.
//

#ifndef COROUTINE_TASK_H
#define COROUTINE_TASK_H

#include <memory>
#include "../base/Timestamp.h"
#include "../Coroutine/coroutine.h"

using namespace common;

namespace coroutine{
    enum AwaitMode{


    };

    class Task{
        friend class Scheduler;
    public:
        typedef std::any ResumeObject;

        Timestamp runtime_;
        Timestamp delta_;
        AwaitMode awaitMode_;

        explicit Task(const Entity& entity)
                :runtime_(),
                //TODO    fixme
                 entity_(entity),
                 delta_()
        {
        }
        bool operator<(const Task& b) const  {
            return this->runtime_ < b.runtime_;
        }
        friend	bool operator <(const shared_ptr< Task>& left, const shared_ptr< Task>&  right)
        {
            return left->runtime_<right->runtime_;
        }

        void invoke(){
            entity_.invoke();
        }

        AwaitMode GetAwaitMode(){
            return awaitMode_;
        }
    private:
        ResumeObject resumeObject_;
        Entity entity_;
    };
}

#endif //COROUTINE_TASK_H
