//
// Created by os on 5/20/24.
//

#ifndef PROJECT_BASE_V1_1_BUFFER_HPP
#define PROJECT_BASE_V1_1_BUFFER_HPP

#include "../h/semaphore.hpp"

class Bafer {
private:
    static const uint64 CAPACITY_BUFFER = 256;
    int *buffer;
    int head, tail;

    sem_t spaceAvailable;
    sem_t itemAvailable;
    sem_t mutexHead;
    sem_t mutexTail;

public:
    Bafer();
    ~Bafer();

    void put(char c);
    char get();

    int getCnt();
};


#endif //PROJECT_BASE_V1_1_BUFFER_HPP