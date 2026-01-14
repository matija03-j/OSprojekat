//
// Created by os on 5/5/24.
//

#include "../h/scheduler.hpp"

List<TCB> Scheduler::readyThreadQueue;

TCB *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *tcb)
{
    readyThreadQueue.addLast(tcb);
}

TCB* Scheduler::prvi() {
    return readyThreadQueue.peekFirst();
}
TCB* Scheduler::zadnji() {
    return readyThreadQueue.peekLast();
}

int Scheduler::size(){
    return readyThreadQueue.size();
}

List<TCB>::Elem* Scheduler::pisi() {
    List<TCB>::Elem* i = readyThreadQueue.pisi_id();
    return i;
}