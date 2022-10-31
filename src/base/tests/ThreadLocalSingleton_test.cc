#include "../ThreadLocalSingleton.h"
#include "../CurrentThread.h"
#include "../Thread.h"

#include <stdio.h>

class Test : common::noncopyable
{
 public:
  Test()
  {
    printf("tid=%d, constructing %p\n", common::CurrentThread::tid(), this);
  }

  ~Test()
  {
    printf("tid=%d, destructing %p %s\n", common::CurrentThread::tid(), this, name_.c_str());
  }

  const common::string& name() const { return name_; }
  void setName(const common::string& n) { name_ = n; }

 private:
  common::string name_;
};

void threadFunc(const char* changeTo)
{
  printf("tid=%d, %p name=%s\n",
         common::CurrentThread::tid(),
         &common::ThreadLocalSingleton<Test>::instance(),
         common::ThreadLocalSingleton<Test>::instance().name().c_str());
  common::ThreadLocalSingleton<Test>::instance().setName(changeTo);
  printf("tid=%d, %p name=%s\n",
         common::CurrentThread::tid(),
         &common::ThreadLocalSingleton<Test>::instance(),
         common::ThreadLocalSingleton<Test>::instance().name().c_str());

  // no need to manually delete it
  // common::ThreadLocalSingleton<Test>::destroy();
}

int main()
{
  common::ThreadLocalSingleton<Test>::instance().setName("main one");
  common::Thread t1(std::bind(threadFunc, "thread1"));
  common::Thread t2(std::bind(threadFunc, "thread2"));
  t1.start();
  t2.start();
  t1.join();
  printf("tid=%d, %p name=%s\n",
         common::CurrentThread::tid(),
         &common::ThreadLocalSingleton<Test>::instance(),
         common::ThreadLocalSingleton<Test>::instance().name().c_str());
  t2.join();

  pthread_exit(0);
}
