//
// Created by jcbjcbjc on 11/23/22.
//

#ifndef COROUTINE_TASK_H
#define COROUTINE_TASK_H

#include <memory>
#include "base/Timestamp.h"
#include "Coroutine/coroutine.h"

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

<<<<<<< Updated upstream
        AwaitMode GetAwaitMode(){
            return awaitMode_;
        }
=======
        Timestamp GetAwaitTimeout(){return entity_.awaitTimeout_;}


        void yield(){entity_.yield(0);}

        void invoke(){entity_.invoke();}


        uint64_t GetTaskId(){return taskId_;}


        AwaitMode GetAwaitMode(){return entity_.awaitMode_;}


        void SetAwaitMode(AwaitMode mode){entity_.awaitMode_=mode;}


        template<typename T>
        T& GetResumeObj(){return static_cast<T>(resumeObject_);}


        Scheduler* GetManager(){return mManager_;}


        void AwakeMeFromAwait(std::any resumeObj);


        void SetStatus(TaskStatus status){status_=status;}


        TaskStatus GetStatus(){return status_;}
>>>>>>> Stashed changes
    private:
        ResumeObject resumeObject_;
        Entity entity_;
    };
}

#endif //COROUTINE_TASK_H
