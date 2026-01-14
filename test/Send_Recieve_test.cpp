//
// Created by os on 8/19/24.
//

#include "Send_Recieve_test.hpp"
#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"

Thread* threadA, *threadB, *threadC;

static volatile bool finisheda = false;
static volatile bool finishedb = false;
static volatile bool finishedc = false;

class Workera: public Thread {
    void workerBodya(void* arg);
public:
    Workera():Thread() {}

    void run() override {
        workerBodya(nullptr);
    }
};

class Workerb: public Thread {
    void workerBodyb(void* arg);
public:
    Workerb():Thread() {}

    void run() override {
        workerBodyb(nullptr);
    }
};


class Workerc: public Thread {
    void workerBodyc(void* arg);
public:
    Workerc():Thread() {}

    void run() override {
        workerBodyc(nullptr);
    }
};

void Workera::workerBodya(void *arg) {
    const char* mes = "A -> C\n";
    threadB->send((char*)("A -> B\n"));
    printString(this->recieve());
    threadC->send((char*)mes);
    printString(this->recieve());

    printString("A finished!\n");
    finisheda = true;
}

void Workerb::workerBodyb(void *arg) {
    threadA->send((char*)("B -> A\n"));
    printString(this->recieve());
    threadC->send((char *)("B -> C\n"));
    printString(this->recieve());

    printString("B finished!\n");
    finishedb = true;
}

void Workerc::workerBodyc(void *arg) {
    threadA->send((char*)("C -> A\n"));
    printString(this->recieve());
    threadB->send((char*)("C -> B\n"));
    printString(this->recieve());


    printString("C finished!\n");
    finishedc = true;
}

void Send_Recieve_test(){
    threadA = new Workera();
    threadB = new Workerb();
    threadC = new Workerc();

    threadA->start();
    threadB->start();
    threadC->start();

    while (!(finisheda && finishedb && finishedc)) {
        Thread::dispatch();
    }

}
