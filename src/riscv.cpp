//
// Created by os on 5/5/24.
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/memoryAllocator.hpp"
#include "../h/semaphore.hpp"
#include "../h/console.hpp"

bool Riscv:: isUser = false;
SleepingList Riscv::sleepingListThread;
SemaphoreList Riscv::semaphoreWait;

void Riscv::setMode(bool value) {
    isUser = value;
}

void Riscv::popSppSpie()
{
    if (isUser) {
        mc_sstatus(Riscv::SSTATUS_SPP);
    }
    else {
        ms_sstatus(Riscv::SSTATUS_SPP);
    }
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

using Body = void (*)(void*);

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){
        //ecall eksplicitno pozvan iz korisnickog ili sistemskog rezima
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 code;
        __asm__ volatile("mv %0, a0" : "=r"(code));
        switch (code) {
            case 0x01: // mem_alloc
                size_t numBlocks;
                __asm__ volatile("mv %0, a1" : "=r"(numBlocks));
                void* ptr;
                ptr = MemoryAllocator::mem_alloc(numBlocks*MEM_BLOCK_SIZE);
                //__asm__ volatile("mv a0, %0" : : "r"(ptr));
                __asm__ volatile("mv t0, %0" : : "r"(ptr));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x02: // mem_free
                void* mem;
                __asm__ volatile("mv %0, a1" : "=r"(mem));
                int ret;
                ret = MemoryAllocator::mem_free(mem);
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x11: //thread_create
                TCB** handle;
                Body body;
                void* arg;
                __asm__ volatile("mv %0, a1" : "=r"(handle));
                __asm__ volatile("mv %0, a2" : "=r"(body));
                __asm__ volatile("mv %0, a6" : "=r"(arg));

                *handle = TCB::createThread(body, arg);

                if (*handle == nullptr){
                    ret = -1;
                } else{
                    ret = 0;
                }
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x13: // thread_dispatch
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;

            case 0x12: // thread_exit
                ret = TCB::threadExit();
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x21: //sem_open
                SemaphoreK** sem;
                unsigned init;
                __asm__ volatile("mv %0, a1" : "=r"(sem));
                __asm__ volatile("mv %0, a2" : "=r"(init));
                *sem = SemaphoreK::open(init);

                if (*sem == nullptr){
                    ret = -1;
                } else{
                    ret = 0;
                }
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x22: //sem_close
                SemaphoreK* hendle;
                __asm__ volatile("mv %0, a1" : "=r"(hendle));
                ret = hendle->close();
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x23: //sem_wait
                SemaphoreK* id;
                __asm__ volatile("mv %0, a1" : "=r"(id));
                ret = id->wait();
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x24: //sem_signal
                __asm__ volatile("mv %0, a1" : "=r"(id));
                ret = id->signal();
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x25: //sem_timedwait
                time_t timeout;
                __asm__ volatile("mv %0, a1" : "=r"(id));
                __asm__ volatile("mv %0, a2" : "=r"(timeout));

                ret = id->timedwait(timeout);

                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x26: //sem_trywait
                __asm__ volatile("mv %0, a1" : "=r"(id));
                ret = id->trywait();
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x31: //time_sleep
                time_t time;
                __asm__ volatile("mv %0, a1" : "=r"(time));
                ret = TCB::threadSleep(time);
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x41: //getc
                char c41;
                //c41 = __getc();
                c41 = Console::getCharIN();
                __asm__ volatile("mv t0, %0" : : "r"(c41));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x42: //putc
                char c42;
                __asm__ volatile("ld %0, 0x58(s0)" : "=r"(c42));
                //__putc(c42);
                Console::putCharOUT(c42);
                break;

            case 0x50: //id
                ret = TCB::running->id_get();
                thread_dispatch();
                __asm__ volatile("mv t0, %0" : : "r"(ret));
                __asm__ volatile ("sw t0, 80(s0)");
                break;

            case 0x51: //send
                TCB* nit;
                char* message;
                __asm__ volatile("mv %0, a1" : "=r"(nit));
                __asm__ volatile("mv %0, a2" : "=r"(message));

                nit->send(message);
                break;

            case 0x52: //recieve
            {
                char *mes = nullptr;
                mes = TCB::running->recieve();
                __asm__ volatile("mv t0, %0" : : "r"(mes));
                __asm__ volatile ("sw t0, 80(s0)");
            }
                break;

            default:
                break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);

    }else if(scause == 0x8000000000000001UL){
        // prekid od tajmera
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        sleepingListThread.update();
        sleepingListThread.remove();

        semaphoreWait.update();

        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }
        w_sstatus(sstatus);
        w_sepc(sepc);

    } else if(scause == 0x8000000000000009UL){
        // spoljasnji hardverski prekid(konzola)
        //console_handler();
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        uint64 brojPrekida = plic_claim();
        if(brojPrekida == CONSOLE_IRQ) {
            while (CONSOLE_RX_STATUS_BIT & *((char *) CONSOLE_STATUS)) {
                char c = (*(char *) CONSOLE_RX_DATA);
                Console::putCharIN(c);
            }
        }
        plic_complete((int) brojPrekida);
        w_sstatus(sstatus);
        w_sepc(sepc);
    } else{
         //ostali prekidi
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        printStr("ERROR! SCAUSE:");
        printInt(scause);
        printStr("\n");
        w_sstatus(sstatus);
        w_sepc(sepc);
        return;
    }

}