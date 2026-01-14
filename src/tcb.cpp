//
// Created by os on 4/30/24.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/printing.hpp"

TCB *TCB::running = nullptr;
uint64  TCB::id_cnt = 10;
uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* arg)
{
    TCB* tcb = new TCB(body, arg);
    return tcb;
}

void TCB::yield() {
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile ("ecall");
}

void TCB::dispatch() {
//    printInt(Scheduler::size());
//    for (List<TCB>::Elem* i = Scheduler::pisi(); i != nullptr; i = i->next) {
//        printInt(i->data->id_get());
//    }
//    printStr("\n");
    TCB *old = running;
    //printString(*Scheduler::prvi());
    if (!old->isFinished() && !old->isBlocked() && !old->isSleep()) { Scheduler::put(old); }
    running = Scheduler::get();
    //Riscv::sleepingList.pisi();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
    //thread_exit();
}

int TCB::threadExit() {
    running->setFinished(true);
    dispatch();
    if(running == nullptr) return -1;
    return 0;

}

int TCB::threadSleep(time_t time) {
    if (time < 0) return -1;
    else if(time == 0) {
        return 0;
    }else {
        Riscv::sleepingListThread.add(running, time);
        running->setSleep(true);
        //Riscv::sleepingList.pisi();
        dispatch();
        return 0;
    }
}

void TCB::send(char *message) {
    sendSem->wait();
    this->message = message;
    sem_signal(recieveSem);
}

char *TCB::recieve() {
    sem_wait(running->recieveSem);
    char* mess = running->message;
    running->message = nullptr;
    running->sendSem->signal();
    return mess;
}

void TCB::initSemaphores(){
    sendSem = new SemaphoreK(1);
    sem_open(&recieveSem, 0);
}
