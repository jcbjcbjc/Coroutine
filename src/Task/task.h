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




    class Task :enable_shared_from_this<Task>{
        friend class Scheduler;
    public:
        typedef std::any ResumeObject;


        uint64_t  taskId_;
        Timestamp runtime_;
        Timestamp delta_;

        explicit Task(const Entity& entity)
                :taskId_(TASK_ID_ALLOCATOR.alloc()),
                runtime_(),
                //TODO    fixme
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

        Timestamp GetAwaitTimeout(){
            return entity_.awaitTimeout_;
        }

        void invoke(){
            entity_.invoke();
        }

        uint64_t GetTaskId(){
            return taskId_;
        }

        AwaitMode GetAwaitMode(){
            return entity_.awaitMode_;
        }
        void SetAwaitMode(AwaitMode mode){
            entity_.awaitMode_=mode;
        }
    private:
        ResumeObject resumeObject_;
        Entity entity_;
    };
}

#endif //COROUTINE_TASK_H
