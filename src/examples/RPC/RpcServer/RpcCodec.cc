// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/common/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "protorpc/RpcCodec.h"

#include "base/Logging.h"
#include "Endian.h"
#include "TcpConnection.h"

#include "protorpc/rpc.pb.h"
#include "protorpc/google-inl.h"

using namespace common;
using namespace common::net;

namespace
{
  int ProtobufVersionCheck()
  {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    return 0;
  }
  int dummy __attribute__ ((unused)) = ProtobufVersionCheck();
}

namespace common
{
namespace net
{
const char rpctag [] = "RPC0";
}
}
