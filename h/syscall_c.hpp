//
// Created by os on 5/12/24.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP

#include "../lib/hw.h"


void *mem_alloc(size_t size); //0x01

int mem_free(void* mem); //0x02

class TCB;
typedef TCB *thread_t;

int thread_create(thread_t* handle, void(*start_routine) (void*), void *arg); //0x11

int thread_exit();  //0x12

void thread_dispatch(); //0x13

class Sem;

typedef Sem _sem;

typedef _sem *sem_t;

int sem_open(sem_t *handle, unsigned init); //0x21

int sem_close(sem_t id); //0x22

int sem_wait(sem_t id); //0x23

int sem_signal(sem_t id); //0x24

int sem_timedwait(sem_t id, time_t timeout); //0x25

int sem_trywait(sem_t id); //0x26

typedef unsigned long time_t;
int time_sleep(time_t ms); //0x31

const int EOF = -1;

char getc(); //0x41

void putc(char c); //0x42

int getThreadId(); //0x50

void send(thread_t handle, char* message); //0x51
char* recieve(); // 0x52



#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
