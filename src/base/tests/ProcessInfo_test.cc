#include "../ProcessInfo.h"
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int main()
{
  printf("pid = %d\n", common::ProcessInfo::pid());
  printf("uid = %d\n", common::ProcessInfo::uid());
  printf("euid = %d\n", common::ProcessInfo::euid());
  printf("start time = %s\n", common::ProcessInfo::startTime().toFormattedString().c_str());
  printf("hostname = %s\n", common::ProcessInfo::hostname().c_str());
  printf("opened files = %d\n", common::ProcessInfo::openedFiles());
  printf("threads = %zd\n", common::ProcessInfo::threads().size());
  printf("num threads = %d\n", common::ProcessInfo::numThreads());
  printf("status = %s\n", common::ProcessInfo::procStatus().c_str());
}
