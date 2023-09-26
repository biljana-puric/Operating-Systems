//
// Created by os on 6/24/22.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_H
#define PROJECT_BASE_V1_1_SYSCALL_C_H

#include "../h/MemoryAllocator.h"
#include "../lib/console.h"
#include "../lib/hw.h"

void* mem_alloc(size_t size);
int mem_free(void* addr);

class PCB;
typedef PCB* thread_t;
int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit ();
void thread_dispatch ();

class Sem;
typedef Sem* sem_t;
int sem_open ( sem_t* handle, unsigned init );
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = -1;
char getc ();
void putc (char);

#endif //PROJECT_BASE_V1_1_SYSCALL_C_H
