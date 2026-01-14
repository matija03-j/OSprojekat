//
// Created by os on 9/24/24.
//

#include "Max_Num_Threads.hpp"
#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

class MyA: public Thread {
    void myBodyA(void* arg);
public:
    MyA():Thread() {}

    void run() override {
        myBodyA(nullptr);
    }
};

class MyB: public Thread {
    void myBodyB(void* arg);
public:
    MyB():Thread() {}

    void run() override {
        myBodyB(nullptr);
    }
};

void MyA::myBodyA(void *arg) {
    for (uint64 i = 0; i < 10; i++)
    {
        printString("A: i=");
        printInt(i);
        printString("\n");

        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                /* busy wait */
            }

            thread_dispatch();
        }
    }

    printString("A finished!\n");
}

void MyB::myBodyB(void *arg) {
    for (uint64 i = 0; i < 16; i++)
    {
        printString("B: i=");
        printInt(i);
        printString("\n");

        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                /* busy wait */
            }

            thread_dispatch();
        }
    }

    printString("B finished!\n");
    thread_dispatch();
}

void Max_Num_Of_Threads(){
    Thread* threadA = new MyA();
    Thread* threadB = new MyB();

    threadA->start();
    threadB->start();

    threadA->join();
    threadB->join();
}