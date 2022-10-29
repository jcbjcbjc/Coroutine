#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define COROUTINE_ERROR 10

// you can only use T with types that can be contained in a single register.
template <typename T> class Coroutine {
private:
  uint8_t stack[0x1000];
  uint64_t back;
  uint64_t self;
  uint64_t para[0x10];
  uint64_t rsp;
  uint64_t rbp;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rip;
  bool ended;

public:
  Coroutine(void (*coroutine)())
      : rsp((uint64_t)&back), rbp(rsp), rip((uint64_t)(void *)coroutine),
        ended(false) {
    back = (uint64_t)exit;
    self = (uint64_t)this;
  }
  ~Coroutine() {}

public:
  T invoke() {
    asm("mov 8(%%rbp),%5\n"
        "leave\n"
        "add $8,%%rsp\n"
        "xchg %%rsp,%0\n"
        "xchg %%rbp,%1\n"
        "xchg %%rsi,%2\n"
        "xchg %%rdi,%3\n"
        "xchg %5,%4\n"
        "jmp *%5\n" 
        ::"m"(rsp),"m"(rbp), "m"(rsi), "m"(rdi)
        , "m"(rip), "r"(0ull)
        :"%rsi","%rdi");
    return (T)0;
  }

  template <typename V>
  void param(uint64_t index, V x) {
    *(V *)(para + index) = x;
  }

  bool eof() { return ended; }

public:
  void yield(T x) {
    asm("mov 8(%%rbp),%6\n"
        "leave\n"
        "add $8,%%rsp\n"
        "xchg %%rsp,%0\n"
        "xchg %%rbp,%1\n"
        "xchg %%rsi,%2\n"
        "xchg %%rdi,%3\n"
        "xchg %6,%4\n"
        "mov %5,%%rax\n"
        "jmp *%6\n" ::"m"(rsp),
        "m"(rbp), "m"(rsi), "m"(rdi), "m"(rip), "r"(*(uint64_t *)&x),
        "r"(0ull)
        :"%rsi","rdi");
  }

  void end() {
    ended = true;
    while (true)
      this->yield((T)NULL);
  }

private:
  static void exit() { std::exit(COROUTINE_ERROR); }
};

#define co_begin(self, type, argv)                                     \
  Coroutine<type> *self;                                                       \
  uint64_t *argv;                                                              \
  asm("mov (%%rbp),%0\n"                                                       \
      "lea 24(%0),%1\n"                                                        \
      "mov 16(%0),%0\n"                                                        \
      : "=r"(self), "=r"(argv))

#define co_arg(argv, type) (*(type *)(argv++))

#define co_end(self) (self->end())

#define COROUTINE_FOREACH(comgr, var)                                          \
  for (decltype(comgr.invoke()) var = comgr.invoke(); !comgr.eof();            \
       var = comgr.invoke())

int main() {
  // rsp >= 0x504000;
  // rsp <  0x700000;
  Coroutine<int> temp([]() {
    co_begin(co, int, argv);
    //uint32_t cnt = co_arg(argv, uint32_t);
    co->yield(0);
    co->yield(5);
    //co->yield(cnt);
    //printf("%d\n", cnt);



    //for (int i = 0; i < cnt; i++)
      //co->yield(i);
    co_end(co);
  });

  temp.invoke();

  //temp.param(0, 3);
  //COROUTINE_FOREACH(temp, i) { printf("%d\n", i); }
  return 0;
}
