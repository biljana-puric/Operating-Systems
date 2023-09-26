//
// Created by os on 6/25/22.
//

#ifndef PROJECT_BASE_V1_1_PCB_H
#define PROJECT_BASE_V1_1_PCB_H

#include "../h/Scheduler.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.h"
#include "../h/printing.h"

class PCB{
public:

    ~PCB() {
        delete stack;
    }
    struct Context{
        uint64 ra;
        uint64 sp;
    };
    using Body = void (*)(void*);
    static PCB *createThread(Body body, uint64* stack, void*arg);
    static int thread_create (PCB** handle, void(*start_routine)(void*), void* arg, void* stack_space);
    static int thread_exit ();
    bool isFinished() const { return finished; }
    void setFinished(bool value) { finished = value; }
    static void yield();
    static void contextSwitch(Context *oldContext, Context *runningContext);
    static PCB *running;
    static PCB *idle;
    int getTimeSlice() const { return timeSlice; }
    Context* getContext(){return &context;}
    static void dispatch();
    static int time_sleep(time_t t);
    static int timeSliceCounter;
    uint64 ssp;
private:
    explicit PCB(Body body, uint64*stack, void*arg, int timeSlice) : body(body), args(arg),
            stack(stack), timeSlice(timeSlice),
            context({body != nullptr ? (uint64) &threadWrapper : 0,
                stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0 }), finished(false)
    {
        if (body != nullptr) {
            Scheduler::put(this);
        }
        sst = (uint64*) MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64));
        ssp = (uint64) &sst[DEFAULT_STACK_SIZE];
    }
    Body body;
    void *args;
    uint64 *stack;
    int timeSlice;
    Context context;
    bool finished;
    uint64* sst;

    friend class Riscv;
    static void threadWrapper();
};

#endif //PROJECT_BASE_V1_1_PCB_H
