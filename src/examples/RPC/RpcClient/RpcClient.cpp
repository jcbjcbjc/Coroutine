//
// Created by jia chaobo on 2022/11/24.
//

#include "RpcClient.h"

RpcClient::RpcClient(common::net::EventLoop *loop, const common::net::InetAddress &serverAddr)
: client_(loop, serverAddr, ""),
  mutex_()
{
    client_.setConnectionCallback(
            std::bind(&RpcClient::onConnection, this, _1));
    client_.setMessageCallback(
            std::bind(&RpcClient::onMessage, this, _1,_2,_3));
}

template<typename REQ,typename RES>
void RpcClient::Call(const std::shared_ptr<Task>& mTask_, REQ& request, RES& response) {
    mTask_->SetAwaitMode(AwaitMode::AwaitForNotifyWithTimeout);

    uint64_t seq=RPC_ID_ALLOCATOR.alloc();

    /// TODO add id in the header of the request
    client_.connection()->send(request);

    {
        MutexLockGuard lock(mutex_);
        wait_tasks_[seq]=mTask_;
    }

    /// yield
    mTask_->yield();

    mTask_->SetAwaitMode(AwaitMode::AwaitForNotifyNoTimeout);

    /// response
    response=mTask_->getResumeObj<RES>();
}
void RpcClient::onMessage(const common::net::TcpConnectionPtr &conn, common::net::Buffer *buf,
                          common::Timestamp receiveTime) {
    uint64_t seq=0;
    /// TODO get seq

    if(!wait_tasks_.contains(seq)){
        /// TODO EXCEPTION
    }

    /// TODO parse the response
    std::any resp;


    TaskPtr task;
    {
        MutexLockGuard lock(mutex_);
        task=std::move(wait_tasks_[seq]);
        wait_tasks_.erase(seq);
    }

    /// awake the task
    task->AwakeMeFromAwait(resp);
}

void RpcClient::connect()
{
    client_.connect();
}