//
// Created by os on 4/30/24.
//

#include "../h/tcb.hpp"
#include "../h/memoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/printing.hpp"
#include "../h/syscall_c.hpp"
#include "../h/console.hpp"

extern void Predrok();
extern void userMain();
bool finishedMain = false;

void console_out(void*)
{
    while (true) {
        volatile char status = *((char*)CONSOLE_STATUS);
        while ((status & CONSOLE_TX_STATUS_BIT)) {
            //if(Console::isEmpty()) thread_dispatch();
            char c = Console::getCharOUT();
            *((char*)CONSOLE_TX_DATA) = c;
            status = *((char*)CONSOLE_STATUS);
            //time_sleep(1);
        }
    }
}

void user_wrapper(void*)
{
    printStr("UserMain() started.\n\n");
    userMain();
    printStr("userMain() finished.\n");
    finishedMain = true;
}

//void idleWrapper(void*)
//{
//    while (true) { thread_dispatch(); }
//}

int main(){
    MemoryAllocator::initFreeMem();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Console::initIO();

    TCB* threads[5];

    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];

    //threads[2] = TCB::createThread(&idleWrapper, nullptr);

    threads[1] = TCB::createThread(&console_out, nullptr);

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    Riscv::setMode(true);

    thread_t user;
    thread_create(&user, user_wrapper, nullptr);


    while (!finishedMain) {
        thread_dispatch();
    }

    Riscv::setMode(false);

    printStr("Vratio sam se u main\n");

    while (Console::isEmpty() != true);

    return 0;
}


