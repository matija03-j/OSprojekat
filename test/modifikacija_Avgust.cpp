//
// Created by os on 9/25/24.
//

#include "modifikacija_Avgust.hpp"
#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

static Semaphore* waitAll;

class MyAa: public Thread {
    void myBodyAa(void* arg);
public:
    MyAa():Thread() {}

    void run() override {
        myBodyAa(nullptr);
    }
};



void MyAa::myBodyAa(void *arg) {
    int id = getCurrentThreadId();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 1000; j++) {
            for (int k = 0; k < 3000*id; k++) {

            }
        }
        //sleep(id + 3);
        barrier();
        printString("ID: ");
        printInt(id);
        printString(", a broj iteracija je ");
        printInt(i);
        printString("\n");
    }
    waitAll->signal();
}


void modifikacija_Avgust(){
    printString("Unesite broj niti (1-10)?\n");
    int n = getc() - '0';
    getc();

    Thread* threads[n];

    waitAll = new Semaphore(0);

    for (int i = 0; i < n; i++) {
        threads[i] = new MyAa();
    }

    for (int i = 0; i < n; i++) {
        threads[i]->start();
    }

    for (int i = 0; i < n; i++){
        waitAll->wait();
    }
}