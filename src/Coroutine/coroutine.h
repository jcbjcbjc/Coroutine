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
#include "base/noncopyable.h"
#include "base/Timestamp.h"
#include<memory>
#include <tuple>
#include <any>

using namespace std;

#define COROUTINE_ERROR 10

namespace coroutine{
    enum AwaitMode{
        AwaitNever,
        AwaitForNotifyNoTimeout,
        AwaitForNotifyWithTimeout,
    };


    class Task;

    class Entity{
    public:

        AwaitMode awaitMode_;
        common::Timestamp awaitTimeout_;
        std::weak_ptr<Task> mTask_;
        Entity(){
            awaitMode_=AwaitMode::AwaitNever;
        }
        virtual ~Entity(){}
        virtual int invoke(){}
        virtual bool eof(){}
        virtual void await(){}
        virtual void yield(int x){}
        virtual void end(){}


        void SetTask(const std::weak_ptr<Task>& task){
            mTask_=task;
        }
        //TODO FIXME
        std::shared_ptr<Task> GetTask(){
            std::shared_ptr<Task>ptr (mTask_.lock());
            if(ptr){
                return ptr;
            }else{
                return nullptr;
            }
        }

        void SetAwaitMode(AwaitMode mode){
            awaitMode_=mode;
        }
    };


    template <typename... ARGS>
    class Coroutine :public Entity,common::noncopyable {
    private:
        uint8_t stack[0x1000];
        uint64_t back;
        uint64_t self;
        //uint8_t para[0x100];
        std::tuple<ARGS...> para;
        uint64_t rsp;
        uint64_t rbp;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t rip;
        bool ended;

    public:
        explicit Coroutine(std::function<void()> coroutine /*void (*coroutine)()*/,ARGS&&... args)
                : rsp((uint64_t)&back), rbp(rsp), rip((uint64_t)coroutine.target<void()>()),
                  ended(false) {
            //std::make_tuple(std::forward<Ts>(ts)...);
            para= make_tuple(std::forward<ARGS>(args)...),
                    back = (uint64_t)exit;
            self = (uint64_t)this;
        }
        /*Coroutine(Coroutine&& coroutine){

        }*/
        ~Coroutine() {}

        /*static Coroutine make_coroutine(std::function<void()> coroutine,ARGS&... args){
            return Coroutine(coroutine,std::forward<ARGS>(args)...);
        }*/
        static Coroutine make_coroutine(std::function<void()> coroutine,ARGS&&... args){
            return Coroutine(coroutine,std::forward<ARGS>(args)...);
        }
    public:
        int invoke() override  {
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

        //template<typename T>
        std::tuple<ARGS...>& param() {
            return para;
        }

        bool eof() override { return ended; }

    public:
        void yield(int x) override {
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

        void end() override {
            ended = true;
            //while (true)
            //this->yield((int)NULL);
        }

    private:
        static void exit() { std::exit(COROUTINE_ERROR); }
    };
#define co_param(pointer,index) \
  std::get<index>(pointer->param())

#define co_yield(co,x) \
co->yield(x);

#define co_begin(self,args...)                                     \
  Coroutine<args> *self;                                                       \
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



}
#endif //COROUTINE_COROUTINE_H