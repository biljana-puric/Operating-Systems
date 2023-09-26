//
// Created by os on 6/23/22.
//

#include "../h/riscv.h"
#include "../h/printing.h"

void Riscv::popSppSpie() {
    __asm__ volatile ("csrw sepc, ra");
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 volatile scause = r_scause();
    if (scause == 0x0000000000000009UL || scause == 0x0000000000000008UL) {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        uint64 volatile syscall_code;
        __asm__ volatile("mv %0, a0" : "=r" (syscall_code));
        if (syscall_code == MEM_ALLOC) {
            size_t blocks;
            __asm__ volatile("mv %0, a1" : "=r" (blocks));
            void *ptr = MemoryAllocator::mem_alloc(blocks);
            __asm__ volatile("mv a0, %0" : : "r" (ptr));
            __asm__ volatile ("sd a0, 80(s0)");
        } else if (syscall_code == MEM_FREE) {
            void *ptr;
            __asm__ volatile("mv %0, a1" : "=r" (ptr));
            int ret = MemoryAllocator::mem_free(ptr);
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        } else if (syscall_code == THREAD_CREATE) {
            thread_t *handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            using Body = void (*)(void *);
            Body start_routine;
            __asm__ volatile("mv %0, a2" : "=r" (start_routine));
            void *arg;
            __asm__ volatile("mv %0, a3" : "=r" (arg));
            uint64 *stack;
            __asm__ volatile("mv %0, a7" : "=r" (stack));
            void*stack_space = (void*)stack;
            int ret = PCB::thread_create(handle, start_routine, arg, stack_space);
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        } else if(syscall_code==THREAD_EXIT){
            int ret = PCB::thread_exit();
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        }
        else if(syscall_code==THREAD_DISPATCH){
            PCB::yield();
        }
        else if(syscall_code==SEM_OPEN){
            sem_t* handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            unsigned init;
            __asm__ volatile("mv %0, a2" : "=r" (init));
            int ret = Sem::sem_open(handle, init);
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        }
        else if(syscall_code==SEM_CLOSE){
            sem_t handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            int ret = Sem::sem_close(handle);
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        }
        else if(syscall_code==SEM_WAIT){
            sem_t handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            int ret = Sem::wait(handle);
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        }
        else if(syscall_code==SEM_SIGNAL){
            sem_t handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            int ret = Sem::signal(handle);
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        } else if(syscall_code == TIME_SLEEP){
            time_t t;
            __asm__ volatile("mv %0, a1" : "=r" (t));
            int ret = PCB::time_sleep(t);
            __asm__ volatile("mv a0, %0" : : "r" (ret));
            __asm__ volatile ("sd a0, 80(s0)");
        }
        else if(syscall_code==GETC){
            char c = __getc();
            __asm__ volatile("mv a0, %0" : : "r" (c));
            __asm__ volatile ("sd a0, 80(s0)");
        }
        else if(syscall_code==PUTC){
            char c;
            __asm__ volatile("mv %0, a1" : "=r" (c));
            __putc(c);
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL) {
        PCB::timeSliceCounter++;
        if (PCB::timeSliceCounter >= PCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            PCB::timeSliceCounter=0;
            PCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        mc_sip(SIP_SSIE);
    }
    else if (scause == 0x8000000000000009UL)
    {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        console_handler();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else{
        printInteger(scause);
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
}