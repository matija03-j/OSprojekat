//
// Created by os on 6/7/24.
//
#include "../h/syscall_cpp.hpp"
#include "../test//printing.hpp"
#include "../h/semaphore.hpp"

static Semaphore *waitForAll;
static Semaphore* sem;
//static bool finish = false;

class MyThreads: public Thread{
public:
    void run() override {
        int i = 3;
        while (i > 0){
            int id = getCurrentThreadId();
            int ret = sem->timedWait(id);
            if(ret == 0){
                printStr("-> Entry with id: ");
                printInt(id);
                printStr("\n");
                sleep(id);
                sem->signal();
                printStr("<- Exit with id: ");
                printInt(id);
                printStr("\n");
                i--;
            } else if(ret == -2){
                printStr("ISteklo vreme id: ");
                printInt(id);
                printStr("\n");
            } else if(ret == -1){
                printStr("Semafor unisten\n");
            }
            //i--;
            dispatch();
        }
        waitForAll->signal();
    }

};

void Predrok(){
    waitForAll = new Semaphore(0);
    sem = new Semaphore(1);

    int numThreads = 10;

    Thread* threads[numThreads];

    for (int i = 0; i < numThreads; i++) {
        threads[i] = new MyThreads();
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i]->start();
    }

    //while (finish == false);

    for (int i = 0; i < numThreads; i++) {
        waitForAll->wait();
    }
}