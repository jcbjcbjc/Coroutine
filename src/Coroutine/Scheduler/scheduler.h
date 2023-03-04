//
// Created by jcbjcbjc on 10/30/22.
//

#ifndef COROUTINE_SCHEDULER_H
#define COROUTINE_SCHEDULER_H
#include "Coroutine/coroutine.h"

#include "base/ThreadPool.h"
#include "base/Timestamp.h"
#include "base/Logging.h"

#include "Coroutine/coroutine.h"
#include "base/ThreadPool.h"
#include "base/Timestamp.h"
#include "base/Logging.h"
#include "base/noncopyable.h"
#include "Coroutine/Task/task.h"

#include <set>
#include <any>
#include <memory>
#include <atomic>

using namespace common;
namespace coroutine{

    typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;

    typedef std::set<TaskPtr> ReadyQueue;
    typedef std::set<TaskPtr> RunningQueue;
    typedef std::set<TaskPtr> AwaitQueue;

    const uint32_t MAX_RUN_NUM=10;

    class Scheduler:noncopyable {
    public:
        typedef std::function<void()> Functor;

        Scheduler(size_t thread_num)
                :threadNum_(thread_num),
                 readynum_(0),
                 runningnum_(0),
                 awaitnum_(0),
                 taskIdAllocator_(),
                 looping_(false),
                 quit_(false),
                 threadId_(0),
                 threadPool_(new ThreadPool())
        {}
        ~Scheduler(){}

        void Start(){
            threadPool_->start(threadNum_);

            threadId_=CurrentThread::tid();

            loop();
        }

        TaskPtr CreateEmptyTask(){
            TaskPtr taskPtr(new Task(this));
            return taskPtr;
        }
        void StartTask(const TaskPtr& task){
            /// TODO FINXME
            runInLoop(std::bind(
                    &Scheduler::StartTaskInLoop,this,task
            ));
        }

        template<typename... ARGS>
        void CreateTask(std::function<void()> task /*void (*task)()*/,ARGS&&... args){
            //TODO fixme
            ///analyse args
            //analyseArgs(args...);
            (analyseArgs(args),...);

            //std::tuple<ARGS...> para=make_tuple(std::forward<ARGS>(args)...);
            //int size=std::tuple_size<decltype(para)>();
            //std::apply([this](auto&... arg){(( analyseArgs(arg)),...);},para);

            Entity entity=Coroutine<ARGS...>::make_coroutine(task,std::forward<ARGS>(args)...);

            runInLoop(std::bind(
                    &Scheduler::CreateTaskInLoop,this,entity
            ));
        }
        void CompleteTask(const TaskPtr& task) {
            runInLoop(std::bind(
                    &Scheduler::CompleteTaskInLoop,this,task
            ));
        }
        void AwakeTaskByNotify(const TaskPtr& task){
            runInLoop(std::bind(
                    &Scheduler::AwakeTaskByNotifyInLoop,this,task
            ));
        }
    private:
        void CreateTaskInLoop(const Entity& entity) {
            TaskPtr taskPtr(new Task(this,entity));
            AddTask(taskPtr);
        }
        void StartTaskInLoop(const TaskPtr& task){
            AddTask(task);
        }
        void CompleteTaskInLoop(const TaskPtr& task) {
            ///leave the runningQueue
            runningQueue_.erase(task);
            runningnum_--;
            task->SetStatus(TaskStatus::Nothing);

//TODO timeincrement
            ///update the cost and the rank
            //task->runtime_=task->delta_+task->runtime_;

            if(task->entity_.eof()){
                task->SetStatus(TaskStatus::End);

                LOG_WARN<<"task end";
            }else{
                AddTask(task);
            }
        }

        void AddTask(const TaskPtr& task){
            auto awaitMode=task->GetAwaitMode();
            auto awaitTimeout=task->GetAwaitTimeout();

            switch (awaitMode) {
                case AwaitNever:
                    readyQueue_.insert(task);
                    readynum_++;
                    task->SetStatus(TaskStatus::Ready);

                    break;
                case AwaitForNotifyNoTimeout:
                case AwaitForNotifyWithTimeout:
                    HandleTaskAwaitForNotify(task,awaitMode,awaitTimeout);
                    break;
                default:
                    LOG_WARN<<"lack of awaitMode";
                    break;
            }
        }

        void HandleTaskAwaitForNotify(const TaskPtr& task,AwaitMode awaitMode,Timestamp awaitTimeoutMs){

            awaitQueue_.insert(task);
            awaitnum_++;
            task->SetStatus(TaskStatus::Await);

        }

        void AwakeTaskByNotifyInLoop(const TaskPtr& task){
            auto it= awaitQueue_.find(task);
            if (it !=awaitQueue_.end()){
                awaitQueue_.erase(*it);
                awaitnum_--;

                readyQueue_.insert(task);
                readynum_++;
                task->SetStatus(TaskStatus::Ready);
            }else{
                LOG_WARN<<"can't find awaitTask";
            }
        }

        void runTask(const TaskPtr& task){
            task->SetStatus(TaskStatus::Running);
            threadPool_->run([&](){
                //TODO caltime
                task->entity_.invoke();

                CompleteTask(task);
            });
        }

        void tick(){
            uint32_t num=MAX_RUN_NUM-runningnum_;
            std::vector<TaskPtr> tasks;
            if(num>=readynum_){
                auto it=readyQueue_.begin();
                while(it!=readyQueue_.end()){
                    tasks.push_back(*it);
                    runningQueue_.insert(*it);
                    it=readyQueue_.erase(it);
                }
                runningnum_+=readynum_;
                readynum_=0;
            }else{
                int i=1;
                auto it=readyQueue_.begin();
                while(i<=num){
                    tasks.push_back(*it);
                    runningQueue_.insert(*it);
                    i++;
                    it=readyQueue_.erase(it);
                }
                runningnum_+=num;
                readynum_-=num;
            }
            for(auto& task:tasks){
                runTask(task);
            }
            doPendingFunctors();
        }

        void loop(){
            assert(!looping_);
            assertInLoopThread();
            looping_ = true;
            quit_ = false;  // FIXME: what if someone calls quit() before loop() ?
            LOG_TRACE << "Scheduler " << this << " start looping";

            while(!quit_){
                tick();
            }
        }

        template<typename T>
        void analyseArgs(T& arg){
            if(typeid(arg)== typeid(int)){
                printf("int");
            }
        }

        void quit(){
            quit_ = true;
        }

        ThreadPoolPtr threadPool_;

        uint64_t readynum_;
        uint64_t runningnum_;
        uint64_t awaitnum_;

        size_t threadNum_;

        ReadyQueue  readyQueue_;
        RunningQueue runningQueue_;
        AwaitQueue awaitQueue_;
        TaskIdAllocator taskIdAllocator_;

        void abortNotInLoopThread(){
            LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
                      << " was created in threadId_ = " << threadId_
                      << ", current thread id = " <<  CurrentThread::tid();
        }
        void assertInLoopThread()
        {
            if (!isInLoopThread())
            {
                abortNotInLoopThread();
            }
        }
        bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
        void doPendingFunctors(){
            std::vector<Functor> functors;
            callingPendingFunctors_ = true;

            {
                MutexLockGuard lock(mutex_);
                functors.swap(pendingFunctors_);
            }

            for (const Functor& functor : functors)
            {
                functor();
            }
            callingPendingFunctors_ = false;
        }
        size_t queueSize() const
        {
            MutexLockGuard lock(mutex_);
            return pendingFunctors_.size();
        }
        void queueInLoop(Functor cb)
        {
            {
                MutexLockGuard lock(mutex_);
                pendingFunctors_.push_back(std::move(cb));
            }

            if (!isInLoopThread() || callingPendingFunctors_)
            {
                //wakeup();
            }
        }
        void runInLoop(Functor cb){
            if (isInLoopThread())
            {
                cb();
            }
            else
            {
                queueInLoop(std::move(cb));
            }
        }


        std::atomic<bool> looping_; /* atomic */
        std::atomic<bool> quit_;
        bool callingPendingFunctors_{}; /* atomic */
        pid_t threadId_;
        mutable MutexLock mutex_;
        std::vector<Functor> pendingFunctors_ GUARDED_BY(mutex_);
    };
}
#endif //COROUTINE_SCHEDULER_H