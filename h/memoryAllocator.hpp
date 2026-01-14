//
// Created by os on 5/2/24.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct MemBlock{
    MemBlock* next;
    size_t size;
};

class MemoryAllocator {
private:
    static MemBlock* headFree;
    static MemBlock* headAlloc;

public:
    static void* mem_alloc(size_t size);
    static int mem_free(void* ptr);
    static void initFreeMem();

};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
