//
// Created by os on 5/12/24.
//

#include "../h/syscall_c.hpp"


void* mem_alloc(size_t size) {
    if(size % MEM_BLOCK_SIZE != 0) size += MEM_BLOCK_SIZE - size%MEM_BLOCK_SIZE;
    size_t numBlocks = size / MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %0" : : "r" (numBlocks));
    __asm__ volatile("li a0, 0x01");
    __asm__ volatile("ecall");

    void* ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

int mem_free(void* mem){
    __asm__ volatile("mv a1, %0" : : "r" (mem));
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

int thread_create(thread_t* handle, void(*start_routine) (void*), void *arg){
    __asm__ volatile("mv a6, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x11");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

void thread_dispatch(){
    __asm__ volatile("li a0, 0x13");
    __asm__ volatile("ecall");
}

int thread_exit(){
    __asm__ volatile("li a0, 0x12");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

int sem_open(sem_t* handle, unsigned init){
    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x21");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

int sem_close (sem_t handle){
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x22");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

int sem_wait(sem_t id){
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x23");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

int sem_signal(sem_t id){
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x24");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

int sem_timedwait(sem_t id, time_t timeout){
    __asm__ volatile("mv a2, %0" : : "r" (timeout));
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x25");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

int sem_trywait(sem_t id){
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("li a0, 0x26");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

int time_sleep(time_t t){
    __asm__ volatile("mv a1, %0" : : "r" (t));
    __asm__ volatile("li a0, 0x31");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

char getc(){
    char ret;
    __asm__ volatile("li a0, 0x41");
    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

void putc(char c){
    __asm__ volatile("mv a1, %0" : : "r" (c));
    __asm__ volatile("li a0, 0x42");
    __asm__ volatile("ecall");
}

int getThreadId(){
    __asm__ volatile("li a0, 0x50");
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int) ret;
}

void send(thread_t handle, char* message){
    __asm__ volatile("mv a2, %0" : : "r" (message));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x51");
    __asm__ volatile("ecall");
}

char* recieve(){
    __asm__ volatile("li a0, 0x52");
    __asm__ volatile("ecall");

    char* ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}
