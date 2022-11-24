//
// Created by jia chaobo on 2022/11/24.
//

#include "RpcClient.h"

RpcClient::RpcClient(common::net::EventLoop *loop, const common::net::InetAddress &listenAddr)
: client_(loop,listenAddr,"")
{
    client_.setConnectionCallback(
            std::bind(&RpcClient::onConnection, this, _1));
    client_.setMessageCallback(
            std::bind(&RpcClient::onMessage, this, _1,_2,_3));
}

void RpcClient::Call(const std::shared_ptr<Task>& mTask_, std::any request, std::any response) {
    mTask_->SetAwaitMode(AwaitMode::AwaitForNotifyWithTimeout);

    //client_.connection()->send(request)

    mTask_->yield();

    //response=mTask_->GetResumeObj<int>();
}
void RpcClient::onMessage(const common::net::TcpConnectionPtr &conn, common::net::Buffer *buf,
                          common::Timestamp receiveTime) {}