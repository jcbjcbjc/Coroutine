// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/common/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "../Poller.h"
#include "PollPoller.h"
#include "EPollPoller.h"

#include <stdlib.h>

using namespace common::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  if (::getenv("common_USE_POLL"))
  {
    return new PollPoller(loop);
  }
  else
  {
    return new EPollPoller(loop);
  }
}
