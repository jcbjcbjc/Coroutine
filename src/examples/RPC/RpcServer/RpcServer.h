// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/common/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef common_NET_PROTORPC_RPCSERVER_H
#define common_NET_PROTORPC_RPCSERVER_H

#include "../Common/TcpServer.h"

namespace google {
namespace protobuf {

class Service;

}  // namespace protobuf
}  // namespace google

namespace common
{
namespace net
{

class RpcServer
{
 public:
  RpcServer(EventLoop* loop,
            const InetAddress& listenAddr);

  void setThreadNum(int numThreads)
  {
    server_.setThreadNum(numThreads);
  }

  void registerService(::google::protobuf::Service*);
  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);

  // void onMessage(const TcpConnectionPtr& conn,
  //                Buffer* buf,
  //                Timestamp time);

  TcpServer server_;
  std::map<std::string, ::google::protobuf::Service*> services_;
};

}  // namespace net
}  // namespace common

#endif  // common_NET_PROTORPC_RPCSERVER_H
