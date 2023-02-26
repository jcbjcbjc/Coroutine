//
// Created by jia chaobo on 2022/11/24.
//

#ifndef COROUTINE_RPCCLIENT_H
#define COROUTINE_RPCCLIENT_H

#include "../Common/TcpClient.h"
#include "Coroutine/Task/task.h"
#include <map>

using namespace common::net;
using namespace coroutine;

class RpcIdAllocator{
public:
    RpcIdAllocator()
    :current_(0)
    {}
    uint64_t alloc(){
        uint64_t id=0;
        if(!recycled_.empty()){
            id=recycled_.back();
            recycled_.pop_back();
        }else{
            id=current_++;
        }
        return id;
    }
    void dealloc(uint64_t id){
        if(id>=current_||std::find(recycled_.begin(), recycled_.end(),id)!=recycled_.end()){
            return;
        }
        recycled_.push_back(id);
    }
private:
    uint64_t current_;
    std::vector<uint64_t> recycled_;
};

static RpcIdAllocator RPC_ID_ALLOCATOR=RpcIdAllocator();

class RpcClient{
public:
    RpcClient(common::net::EventLoop *loop, const common::net::InetAddress &serverAddr);
    ~RpcClient();
    void connect();

    template<typename REQ,typename RES>
    void Call(const std::shared_ptr<Task>& mTask_,REQ& request,RES& response);

private:
    void onConnection(const TcpConnectionPtr& conn);

    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf,
                   common::Timestamp receiveTime);


    TcpClient client_;
    MutexLock mutex_;
    std::map<uint64_t ,TaskPtr> wait_tasks_;

};

#endif //COROUTINE_RPCCLIENT_H