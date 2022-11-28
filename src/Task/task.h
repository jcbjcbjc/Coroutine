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
    enum TaskStatus{
        Running,
        Ready,
        Await,
        Nothing,
        End
    };


    class TaskIdAllocator{
    public:
        TaskIdAllocator()
                : recycled(),
                  current(0)
        {
        }

        uint64_t alloc(){
            if(!recycled.empty()){
                uint64_t id =recycled.back();
                recycled.pop_back();
                return id;
            }else{
                return current++;
            }
        }
        void dealloc(uint64_t id){
            if (id>=current){
                return;
            }

            if(std::find(recycled.begin(), recycled.end(), id) != recycled.end()){
                return;
            }

            recycled.push_back(id);
        }
    private:
        std::vector<uint64_t> recycled;
        uint64_t current;
    };




    static TaskIdAllocator TASK_ID_ALLOCATOR=TaskIdAllocator();



    class Scheduler;

    class Task :enable_shared_from_this<Task>{
        friend class Scheduler;
    public:
        typedef std::any ResumeObject;


        uint64_t  taskId_;
        Timestamp runtime_;
        Timestamp delta_;

        explicit Task(Scheduler* manager,const Entity& entity)
                :taskId_(TASK_ID_ALLOCATOR.alloc()),
                 runtime_(),
                //TODO    fixme
                 mManager_(manager),
                 entity_(entity),
                 delta_()
        {
            entity_.SetTask(shared_from_this());
        }
        ~Task(){
            TASK_ID_ALLOCATOR.dealloc(taskId_);
        }
        bool operator<(const Task& b) const  {
            return this->runtime_ < b.runtime_;
        }
        friend	bool operator <(const shared_ptr< Task>& left, const shared_ptr< Task>&  right)
        {
            return left->runtime_<right->runtime_;
        }


        Timestamp GetAwaitTimeout(){return entity_.awaitTimeout_;}


        void invoke(){entity_.invoke();}


        uint64_t GetTaskId(){return taskId_;}


        AwaitMode GetAwaitMode(){return entity_.awaitMode_;}


        void SetAwaitMode(AwaitMode mode){entity_.awaitMode_=mode;}


        Scheduler* GetManager(){return mManager_;}


        void AwakeMeFromAwait(std::any resumeObj);


        void SetStatus(TaskStatus status){status_=status;}


        TaskStatus GetStatus(){return status_;}
    private:
        TaskStatus status_;

        Scheduler* mManager_;
        ResumeObject resumeObject_;
        Entity entity_;
    };
}

#endif //COROUTINE_TASK_H
