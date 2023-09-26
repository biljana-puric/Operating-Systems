//
// Created by os on 6/19/22.
//

#include "../h/MemoryAllocator.h"
#include "../h/printing.h"
#include "../h/riscv.h"
#include "../h/PCB.h"
#include "../h/syscall_c.h"
//#include "../h/Threads_C_API_test.hpp"
//#include "../h/ConsumerProducer_C_API_test.h"
//#include "../h/ThreadSleep_C_API_test.hpp"
//#include "../h/Threads_CPP_API_test.hpp"
//#include "../h/ConsumerProducer_CPP_Sync_API_test.hpp"

void loop(void* arg){
    while(1){
        thread_dispatch();
    }
}
bool userFinished = false;
void userMain(void* arg){
    //__asm__ volatile ("csrw sepc, ra");

    //Threads_C_API_test();
    //Threads_CPP_API_test();
    //producerConsumer_C_API();
    //producerConsumer_CPP_Sync_API();
    userFinished=true;
}
int main()
{

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    thread_t thread;
    uint64* stack = (uint64*) MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE * sizeof (uint64));
    PCB::thread_create(&thread, nullptr, nullptr, stack);
    PCB::running = thread;

    thread_t idle;
    uint64 *stack_idle = (uint64*) MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE*sizeof (uint64));
    PCB::thread_create(&idle, loop, nullptr, stack_idle);
    PCB::idle = idle;
    thread_t userM;
    uint64 *stack_user = (uint64*) MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE*sizeof (uint64));
    PCB::thread_create(&userM, userMain, nullptr, stack_user);

    while(true){
        if(!userFinished){
            PCB::dispatch();
        }
        else{
            break;
        }
    }

    return 0;

}




