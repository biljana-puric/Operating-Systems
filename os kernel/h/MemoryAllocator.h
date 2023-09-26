//
// Created by os on 6/19/22.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_H

#include "../lib/hw.h"

struct FreeMem{
    size_t size;
    FreeMem* next;
    FreeMem* prev;
};

class MemoryAllocator{

public:
    static void* mem_alloc (size_t size);
    static int mem_free (void *addr);

private:
    static FreeMem* head;
    MemoryAllocator() {};

};
#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
