//
// Created by jcbjcbjc on 10/30/22.
//

#ifndef COROUTINE_COROUTINE_H
#define COROUTINE_COROUTINE_H
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tuple>

using namespace std;

#define COROUTINE_ERROR 10

 class Coroutine {
private:
    uint8_t stack[0x1000];
    uint64_t back;
    uint64_t self;
    std::tuple<> para;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rip;
    bool ended;

public:
     template <typename... ARGS>
    Coroutine(void (*coroutine)(),ARGS&&... args)
            : rsp((uint64_t)&back), rbp(rsp), rip((uint64_t)(void *)coroutine),
              ended(false) {
        para= std::make_tuple<ARGS...>(args...);
        back = (uint64_t)exit;
        self = (uint64_t)this;

    }
    ~Coroutine() {}

    template <typename... ARGS>
    static Coroutine make_coroutine(void (*coroutine)(),ARGS&&... args){
       return Coroutine(coroutine,args...);
    }
public:
    int invoke() {
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
        return (int)0;
    }

    //template <typename V>
    /*void param(uint64_t index, V x) {
        *(V *)(para + index) = x;
    }*/

    bool eof() { return ended; }

public:
    void yield(int x) {
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
            this->yield((int)NULL);
    }

private:
    static void exit() { std::exit(COROUTINE_ERROR); }
};
#define co_begin(self)                                     \
  Coroutine *self;                                                       \
  asm("mov (%%rbp),%0\n"                                                       \
      "mov 16(%0),%0\n"                                                        \
      : "=r"(self))
/*#define co_begin(self, type...)                                     \
  Coroutine<type> *self;                                                       \
  asm("mov (%%rbp),%0\n"                                                       \
      "mov 16(%0),%0\n"                                                        \
      : "=r"(self))*/

//#define co_arg(argv, type) (*(type *)(argv++))

#define co_end(self) (self->end())

#define COROUTINE_FOREACH(comgr, var)                                          \
  for (decltype(comgr.invoke()) var = comgr.invoke(); !comgr.eof();            \
       var = comgr.invoke())


#endif //COROUTINE_COROUTINE_H
