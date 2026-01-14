//
// Created by os on 5/20/24.
//

#include "../h/buffer.hpp"
#include "../h/syscall_c.hpp"

Bafer::Bafer() : head(0), tail(0) {
    buffer = (int *)mem_alloc(sizeof(int) * CAPACITY_BUFFER);
    sem_open(&itemAvailable, 0);
    sem_open(&spaceAvailable, CAPACITY_BUFFER);
    sem_open(&mutexHead, 1);
    sem_open(&mutexTail, 1);
}

Bafer::~Bafer() {
    while (getCnt() > 0) {
        char ch = buffer[head];
        putc(ch);
        head = (head + 1) % CAPACITY_BUFFER;
    }
    putc('!');
    putc('\n');

    mem_free(buffer);
    sem_close(itemAvailable);
    sem_close(spaceAvailable);
    sem_close(mutexTail);
    sem_close(mutexHead);
}


void Bafer::put(char c) {
    sem_wait(spaceAvailable);

    sem_wait(mutexTail);
    buffer[tail] = c;
    tail = (tail + 1) % CAPACITY_BUFFER;
    sem_signal(mutexTail);

    sem_signal(itemAvailable);

}

char Bafer::get() {
    sem_wait(itemAvailable);

    sem_wait(mutexHead);

    int ret = buffer[head];
    head = (head + 1) % CAPACITY_BUFFER;
    sem_signal(mutexHead);

    sem_signal(spaceAvailable);

    return ret;
}

int Bafer::getCnt() {
    int ret;

    sem_wait(mutexHead);
    sem_wait(mutexTail);

    if (tail >= head) {
        ret = tail - head;
    } else {
        ret = CAPACITY_BUFFER - head + tail;
    }

    sem_signal(mutexTail);
    sem_signal(mutexHead);

    return ret;
}