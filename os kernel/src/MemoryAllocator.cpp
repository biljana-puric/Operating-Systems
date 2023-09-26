//
// Created by os on 6/19/22.
//
#include "../h/MemoryAllocator.h"
#include "../lib/console.h"
#include "../h/print.h"

FreeMem* MemoryAllocator::head=nullptr;

int tryToJoin(FreeMem* cur){
    if(!cur) return 0;
    if(cur->next && ((size_t)(cur) + cur->size == (size_t)(cur->next))){
        cur->size+=cur->next->size;
        cur->next=cur->next->next;
        if(cur->next) cur->next->prev = cur;
        return 1;
    }
    else{
        return 0;
    }
}

void *MemoryAllocator::mem_alloc(size_t size) {
    size_t blocks = (size + sizeof (size_t))/MEM_BLOCK_SIZE;
    size_t excess = ((size + sizeof(size_t)) %MEM_BLOCK_SIZE) ? 1:0;
    size = blocks + excess;
    size=size*MEM_BLOCK_SIZE;
    if(!head) {
        head=(FreeMem*) HEAP_START_ADDR;
        head->size= (size_t)HEAP_END_ADDR-(size_t)HEAP_START_ADDR;
        head->next= nullptr;
        head->prev = nullptr;
    }
    size_t sz = size;
    for(FreeMem* cur=head; cur!= nullptr; cur=cur->next){
        if(cur->size==sz) {
            if(cur->prev){
                cur->prev->next=cur->next;
                if(cur->next) cur->next->prev=cur->prev;
                *((char*)cur)=(char)sz;
                void* ret = (void*)((char*)cur+sizeof(size_t));
                return ret;
            }
            else{
                if(cur->next) {
                    head = cur->next;
                    head->prev = nullptr;
                }
                *((char*)cur)=(char)sz;
                void* ret = (void*)((char*)cur+sizeof(size_t));
                return ret;
            }
        }
        else {
            if (cur->size < sz) continue;
            if (cur->prev) {
                size_t s = cur->size - sz;
                FreeMem *temp1 = cur->next;
                FreeMem *temp2 = cur->prev;
                FreeMem *free = cur;
                cur = (FreeMem *) ((size_t) (cur) + sz);
                cur->size = s;
                cur->next = temp1;
                cur->prev = temp2;
                cur->prev->next = cur;
                if (cur->next) cur->next->prev = cur;
                *((char*)free) = (char)sz;
                void *ret = (void *) ((char *) free + sizeof(size_t));
                return ret;
            } else {

                size_t s = head->size - sz;
                FreeMem *temp = head->next;
                head = (FreeMem *) ((size_t) (head) + sz);
                head->size = s;
                head->next = temp;
                head->prev = nullptr;
                if (head->next) head->next->prev = head;
                *((char*)cur) = (char)sz;
                void *ret = (void *) ((char *) cur + sizeof(size_t));
                return ret;

            }
        }
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void * addr) {
    if(!addr) return 1;
    FreeMem* cur= nullptr;
    if(!head || addr < (void*)head)
        cur= nullptr;
    else
        for(cur=head; cur->next!= nullptr && addr>(void*)(cur->next); cur=cur->next);
    FreeMem* newSeg = (FreeMem*)((char*)addr - sizeof(size_t));
    newSeg->size = *((char*)(newSeg));
    newSeg->prev = cur;
    if (cur) newSeg->next = cur->next;
    else newSeg->next = head;
    if (newSeg->next) newSeg->next->prev = newSeg;
    if (cur) cur->next = newSeg;
    else head = newSeg;

    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0;
}

