#include "LogFile.h"
#include "Logging.h"

#include <unistd.h>

std::unique_ptr<common::LogFile> g_logFile;

void outputFunc(const char* msg, int len)
{
  g_logFile->append(msg, len);
}

void flushFunc()
{
  g_logFile->flush();
}

int main(int argc, char* argv[])
{
  char name[256] = { '\0' };
  strncpy(name, argv[0], sizeof name - 1);
  g_logFile.reset(new common::LogFile(::basename(name), 200*1000));
  common::Logger::setOutput(outputFunc);
  common::Logger::setFlush(flushFunc);

  common::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  for (int i = 0; i < 10000; ++i)
  {
    LOG_INFO << line << i;

    usleep(1000);
  }
}
