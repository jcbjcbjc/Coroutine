#include "../Singleton.h"
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

class TestNoDestroy : common::noncopyable
{
 public:
  // Tag member for Singleton<T>
  void no_destroy();

  TestNoDestroy()
  {
    printf("tid=%d, constructing TestNoDestroy %p\n", common::CurrentThread::tid(), this);
  }

  ~TestNoDestroy()
  {
    printf("tid=%d, destructing TestNoDestroy %p\n", common::CurrentThread::tid(), this);
  }
};

void threadFunc()
{
  printf("tid=%d, %p name=%s\n",
         common::CurrentThread::tid(),
         &common::Singleton<Test>::instance(),
         common::Singleton<Test>::instance().name().c_str());
  common::Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
  common::Singleton<Test>::instance().setName("only one");
  common::Thread t1(threadFunc);
  t1.start();
  t1.join();
  printf("tid=%d, %p name=%s\n",
         common::CurrentThread::tid(),
         &common::Singleton<Test>::instance(),
         common::Singleton<Test>::instance().name().c_str());
  common::Singleton<TestNoDestroy>::instance();
  printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));
}
