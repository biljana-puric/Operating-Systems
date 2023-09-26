//
// Created by os on 7/15/22.
//
#include "../h/MemoryAllocator.h"
#include "../lib/hw.h"
void *operator new(size_t n) {
    return MemoryAllocator::mem_alloc(n);
}

void *operator new[](size_t n) {
    return MemoryAllocator::mem_alloc(n);
}

void operator delete(void *p) noexcept {
    MemoryAllocator::mem_free(p);
}

void operator delete[](void *p) noexcept{
    MemoryAllocator::mem_free(p);
}
