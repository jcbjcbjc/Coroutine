//
// Created by jia chaobo on 2022/11/24.
//

#ifndef COROUTINE_RPCCLIENT_H
#define COROUTINE_RPCCLIENT_H

#include "../Common/TcpClient.h"
#include "Task/task.h"

using namespace common::net;
using namespace coroutine;

class RpcClient{
public:
    RpcClient(EventLoop* loop,
              const InetAddress& listenAddr);
    ~RpcClient();
    void Call(const std::shared_ptr<Task>& mTask_,std::any request,std::any response);

private:
    void onConnection(const TcpConnectionPtr& conn);

    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf,
                   common::Timestamp receiveTime);


    TcpClient client_;


};

#endif //COROUTINE_RPCCLIENT_H