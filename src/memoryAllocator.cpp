//
// Created by os on 5/2/24.
//

#include "../h/memoryAllocator.hpp"
#include "../h/tcb.hpp"
#define BLOCK_HEADER_SIZE (sizeof(MemBlock))

MemBlock* MemoryAllocator::headFree = nullptr;
MemBlock* MemoryAllocator::headAlloc = nullptr;



void MemoryAllocator::initFreeMem() {
    MemoryAllocator::headFree = (MemBlock*) HEAP_START_ADDR;
    MemoryAllocator::headFree->next = nullptr;
    MemoryAllocator::headFree->size = ((uint64) HEAP_END_ADDR - (uint64)HEAP_START_ADDR) - sizeof(MemBlock);
    MemoryAllocator::headAlloc = nullptr;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size == 0) return nullptr;
    if(size % MEM_BLOCK_SIZE != 0) size += MEM_BLOCK_SIZE - size%MEM_BLOCK_SIZE;
    MemBlock* prevFree = nullptr;
    MemBlock* currentFree = headFree;

    // Trazenje slobodnog bloka
    while (currentFree != nullptr && currentFree->size < size) {
        prevFree = currentFree;
        currentFree = currentFree->next;
    }

    if (currentFree == nullptr) {
        return nullptr;
    }

    if (currentFree->size > size + BLOCK_HEADER_SIZE) {
        MemBlock* newBlock = (MemBlock*)((char*)currentFree + BLOCK_HEADER_SIZE + size);
        newBlock->size = currentFree->size - size - BLOCK_HEADER_SIZE;
        newBlock->next = currentFree->next;
        currentFree->size = size;
        currentFree->next = newBlock;
        if (prevFree != nullptr) {
            prevFree->next = newBlock;
        } else {
            headFree = newBlock;
        }
    } else {
        // Izbacivanje bloka iz liste slobodnih ako tacno odgorava
        if (prevFree != nullptr) {
            prevFree->next = currentFree->next;
        } else {
            headFree = currentFree->next;
        }
    }

    // dodavanje u listu zauzetih
    currentFree->next = headAlloc;
    headAlloc = currentFree;

    return (void*) ((char*)headAlloc + BLOCK_HEADER_SIZE);
}

int MemoryAllocator::mem_free(void *ptr) {
    if (ptr == nullptr) {
        return -1;
    }

    MemBlock* block = (MemBlock*)((char*)ptr - BLOCK_HEADER_SIZE);

    // Izbacivanje bloka iz liste zauzetih
    MemBlock* currentAlloc = headAlloc;
    MemBlock* prevAlloc = nullptr;

    while (currentAlloc != nullptr && currentAlloc != block) {
        prevAlloc = currentAlloc;
        currentAlloc = currentAlloc->next;
    }

    if (currentAlloc == nullptr) {
        return -2;
    }

    if (prevAlloc != nullptr) {
        prevAlloc->next = currentAlloc->next;
    } else {
        headAlloc = currentAlloc->next;
    }

    // Dodavanje u listu slobodnih
    MemBlock* prevFree = nullptr;
    MemBlock* currentFree = headFree;
    while (currentFree != nullptr && currentFree < block) {
        prevFree = currentFree;
        currentFree = currentFree->next;
    }

    // Spoj sa predhodnim blokom
    if (prevFree != nullptr && (char*)prevFree + BLOCK_HEADER_SIZE + prevFree->size == (char*)block) {
        prevFree->size += BLOCK_HEADER_SIZE + block->size;
        block = prevFree;
    } else {
        block->next = currentFree;
        if (prevFree != nullptr) {
            prevFree->next = block;
        } else {
            headFree = block;
        }
    }

    // Spoj sa sledecim
    if (currentFree != nullptr && (char*)block + BLOCK_HEADER_SIZE + block->size == (char*)currentFree) {
        block->size += BLOCK_HEADER_SIZE + currentFree->size;
        block->next = currentFree->next;
    }
    return 0;
}
