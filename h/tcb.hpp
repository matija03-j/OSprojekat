//
// Created by os on 4/30/24.
//

#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/syscall_c.hpp"

class SemaphoreK;

class TCB {

public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }
    void setBlocked(bool value) { blocked = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body, void* arg);
    static int threadExit();

    static void yield();

    static TCB *running;

    int id_get(){ return id;}

    static int threadSleep(time_t time);

    bool isSleep() const { return sleep; }
    void setSleep(bool value) { sleep = value; }

    void send(char* message);
    char* recieve();
    void initSemaphores();

    int retSemWait = -5;

private:
    TCB(Body body, void* arg) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            arg(arg), finished(false), blocked(false), timeSlice(DEFAULT_TIME_SLICE), sleep(false)//, sem(3)
    {
        if (body != nullptr ){
            Scheduler::put(this);
        }
        id = id_cnt++;
        initSemaphores();
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };


    Body body;
    uint64 *stack;
    Context context;
    void* arg;
    bool finished;
    int id;
    bool blocked;
    uint64 timeSlice;
    bool sleep;

    //message
    SemaphoreK* sendSem;
    sem_t recieveSem;
    char* message = nullptr;
    //--------

    friend class Riscv;

    static void threadWrapper();
    static void contextSwitch(Context *oldContext, Context *runningContext);
    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 id_cnt;
};

#endif //PROJECT_BASE_V1_1_TCB_HPP
