//
// Created by os on 6/25/22.
//
#include "../h/PCB.h"
#include "../h/riscv.h"

PCB *PCB::running = nullptr;
PCB *PCB::idle = nullptr;
int PCB::timeSliceCounter = 0;

PCB *PCB::createThread(Body body, uint64* stack, void* arg)
{
    return new PCB(body, stack, arg, DEFAULT_TIME_SLICE);
}

void PCB::yield()
{
    Riscv::pushRegisters();

    dispatch();

    Riscv::popRegisters();
}

void PCB::dispatch()
{
    PCB *old = running;
    if (!old->isFinished()) {
        Scheduler::put(old);
    }
    running = Scheduler::get();

    PCB::contextSwitch(&old->context, &running->context);
}

int PCB::thread_create(PCB **handle, void (*start_routine)(void *), void *arg, void *stack_space) {
    PCB* pcb = createThread(start_routine, (uint64*)stack_space, arg);
    *handle = pcb;
    return 0;
}

void PCB::threadWrapper() {
    //Riscv::popSppSpie();
    running->body(running->args);
    running->setFinished(true);
    thread_dispatch();
}

int PCB::thread_exit() {
    running->setFinished(true);
    yield();
    return 0;
}

int PCB::time_sleep(time_t t) {
    //SleepingThreads::addLast(running, t);
    PCB *old = running;
    running = Scheduler::get();
    PCB::contextSwitch(&old->context, &running->context);
    return 0;
}
