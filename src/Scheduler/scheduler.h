//
// Created by jcbjcbjc on 10/30/22.
//

#ifndef COROUTINE_SCHEDULER_H
#define COROUTINE_SCHEDULER_H
#include "../Coroutine/coroutine.h"
#include "../base/ThreadPool.h"
#include "../base/Timestamp.h"
#include "../base/Logging.h"
#include <set>
#include <any>
#include <memory>
#include <atomic>

using namespace common;

struct Task{
    Timestamp runtime_;
    Coroutine co_;

    Timestamp delta_;
    //TODO    compare

    Task(const Coroutine& coroutine)
    :runtime_(),
    //TODO    fixme
    co_(coroutine),
    delta_()
    {
    }

};
typedef std::shared_ptr<ThreadPool> ThreadPoolPtr;
typedef std::shared_ptr<Task> TaskPtr;
typedef std::set<TaskPtr> ReadyQueue;
typedef std::set<TaskPtr> RunningQueue;
typedef std::set<TaskPtr> SleepQueue;

const uint32_t MAX_RUN_NUM=10;

class Scheduler {
public:
    typedef std::function<void()> Functor;

    Scheduler(size_t thread_num)
    :threadNum_(thread_num),
    readynum_(0),
    runningnum_(0),
    sleepnum_(0),
    looping_(false),
     quit_(false),
     threadId_(0),
     threadPool_()
    {}
    ~Scheduler(){}

    void Start(){
        threadPool_->start(threadNum_);

        threadId_=CurrentThread::tid();

        loop();
    }

    template<typename... ARGS>
    void CreateTask(void (*task)(),ARGS... args){
        Coroutine co=Coroutine::make_coroutine(task,args...);

        runInLoop(std::bind(
                &Scheduler::CreateTaskInLoop,this,co
                ));
    }
    void CompleteTask(const TaskPtr& task) {
        runInLoop(std::bind(
                &Scheduler::CompleteTaskInLoop,this,task
                ));
    }
private:
    void CreateTaskInLoop(const Coroutine& coroutine) {
        TaskPtr taskPtr(new Task(coroutine));
        readyQueue_.insert(taskPtr);
        readynum_++;
    }
    void CompleteTaskInLoop(const TaskPtr& task) {
        //TODO timeincrement
        //task->runtime_=task->delta_+task->runtime_;
        runningQueue_.erase(task);
        runningnum_--;
        if(task->co_.eof()){

        }else{
            readyQueue_.insert(task);
            readynum_++;
        }
    }

    void runTask(const TaskPtr& task){
        threadPool_->run([&](){
            //TODO caltime
            task->co_.invoke();

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

    void quit(){
        quit_ = true;
    }

    ThreadPoolPtr threadPool_;

    uint64_t readynum_;
    uint64_t runningnum_;
    uint64_t sleepnum_;

    size_t threadNum_;

    ReadyQueue  readyQueue_;
    RunningQueue runningQueue_;
    SleepQueue sleepQueue_;



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

#endif //COROUTINE_SCHEDULER_H
