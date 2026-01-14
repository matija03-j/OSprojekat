//
// Created by os on 5/15/24.
//

#include "../h/semaphore.hpp"
#include "../h/riscv.hpp"

class TCB;

void SemaphoreK::block() {
    if (!TCB::running->isBlocked()){
        TCB::running->setBlocked(true);
        blockedQueue.addLast(TCB::running);
        TCB::yield();
    }
}

void SemaphoreK::unblock() {
    TCB* t = blockedQueue.removeFirst();
    t->setBlocked(false);
    if(!waitList.isEmpty()){
        waitList.removeThread(t);
    }
    t->retSemWait = 0;
    Scheduler::put(t);
}

int SemaphoreK::wait() {
    if(!this->isOpen) return -1;
    this->val--;
    if(this->val < 0) {
        this->block();
    }
    return 0;
}

int SemaphoreK::signal() {
    if(!this->isOpen) return -1;
    this->val++;
    if(this->val <= 0) {
        this->unblock();
    }
    return 0;
}

SemaphoreK* SemaphoreK::open(unsigned init) {
    SemaphoreK* sem = new SemaphoreK(init);
    sem->isOpen = true;
    return sem;
}

int SemaphoreK::close() {
    if(!isOpen) return -1;
    while (this->blockedQueue.peekFirst()) {
        this->blockedQueue.peekFirst()->setBlocked(false);
        this->blockedQueue.peekFirst()->retSemWait = -1;
        Scheduler::put(this->blockedQueue.peekFirst());
        this->waitList.removeThread(this->blockedQueue.peekFirst());
        this->blockedQueue.removeFirst();
    }
    this->isOpen = false;
    if(mem_free(this)) return -2;
    return 0;
}

int SemaphoreK::timedwait(time_t timeout) {
    if(this->val - 1 >= 0) {
        this->val--;
        return 0;
    }else {
        if(!Riscv::semaphoreWait.find(this)){
            Riscv::semaphoreWait.add(this);
        }
        waitList.add(TCB::running, timeout);
        this->wait();
        if(this->isOpen){
            if(TCB::running->retSemWait < 0){
                this->removeFromBlocked(TCB::running);
                this->val++;
                return -2;
            } else{
                TCB::running->setSleep(false);
                return 0;
            }
        }else{
            TCB::running->setSleep(false);
            return -1;
        }
    }
}


int SemaphoreK::trywait() {
    if(!this->isOpen) return -1;
    if (this->val - 1 < 0) return 0;
    else {
        this->wait();
        return 1;
    }
}


void SemaphoreK::removeFromBlocked(TCB* t) {
    blockedQueue.removeData(t);
}