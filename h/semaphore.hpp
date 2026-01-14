//
// Created by os on 5/15/24.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHORE_HPP
#define PROJECT_BASE_V1_1_SEMAPHORE_HPP

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/sleepingList.hpp"


class SemaphoreK {
public:
    SemaphoreK(unsigned init = 1): val((int)init), isOpen(true){}

    int wait ();
    int signal();

    int get_value() const {return val; }

    static SemaphoreK* open(unsigned init);
    int close();

    int trywait();
    int timedwait(time_t timeout);

    SleepingList* getWaitList(){ return &waitList; }
    void removeFromBlocked(TCB* t);

protected:
    void block();
    void unblock();

private:
    int val;
    bool isOpen;
    List<TCB> blockedQueue;
    SleepingList waitList;
};


#endif //PROJECT_BASE_V1_1_SEMAPHORE_HPP
