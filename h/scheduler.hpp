//
// Created by os on 5/5/24.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List<TCB> readyThreadQueue;

public:
    static TCB *get();

    static void put(TCB *tcb);

    static int size();

    static List<TCB>::Elem* pisi();

    static TCB* prvi();
    static TCB* zadnji();

};


#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
