//
// Created by os on 9/4/22.
//

#include "../h/syscall_c.h"
#include "../h/riscv.h"
#include "../lib/hw.h"

void* mem_alloc(size_t size){
    int syscall_code = MEM_ALLOC;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));
    __asm__ volatile("ecall");
    void* ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}
int mem_free(void* addr){
    int syscall_code = MEM_FREE;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r" (syscall_code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    int syscall_code = THREAD_CREATE;
    thread_t * h;
    __asm__ volatile("mv %0, a0" : "=r" (h));
    using Body = void(*)(void*);
    Body sr;
    __asm__ volatile("mv %0, a1" : "=r" (sr));
    void* a;
    __asm__ volatile("mv %0, a2" : "=r" (a));
    uint64* stack = (uint64*) MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile("mv a1, %0" : : "r" (stack));
    __asm__ volatile ("mv a1, %0" : : "r" (h)); //handle
    __asm__ volatile ("mv a2, %0" : : "r" (sr)); //rutina
    __asm__ volatile ("mv a3, %0" : : "r" (a)); //arg
    __asm__ volatile ("mv a7, a0"); //stek
    __asm__ volatile ("mv a0, %0" : : "r" (syscall_code)); //kod
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_exit (){
    int syscall_code = THREAD_EXIT;
    __asm__ volatile("mv a0, %0" : : "r" (syscall_code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void thread_dispatch (){
    int syscall_code = THREAD_DISPATCH;
    __asm__ volatile("mv a0, %0" : : "r" (syscall_code));
    __asm__ volatile("ecall");
}

class Sem;
typedef Sem* sem_t;
int sem_open ( sem_t* handle, unsigned init ){
    int syscall_code = SEM_OPEN;
    sem_t s;
    __asm__ volatile("mv %0, a0" : "=r" (s));
    unsigned i;
    __asm__ volatile("mv %0, a1" : "=r" (i));
    __asm__ volatile("mv a0, %0" : : "r" (syscall_code));
    __asm__ volatile("mv a1, %0" : : "r" (s));
    __asm__ volatile("mv a2, %0" : : "r" (i));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_close (sem_t handle){
    int syscall_code = SEM_CLOSE;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_wait (sem_t id){
    int syscall_code = SEM_WAIT;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_signal (sem_t id){
    int syscall_code= SEM_SIGNAL;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int time_sleep (time_t){
    int syscall_code = TIME_SLEEP;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

char getc (){
    int syscall_code = GETC;
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));
    __asm__ volatile("ecall");
    char ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void putc (char){
    int syscall_code = PUTC;
    __asm__ volatile("mv a1, a0");
    __asm__ volatile("mv a0, %0" : : "r"(syscall_code));
    __asm__ volatile("ecall");
}
