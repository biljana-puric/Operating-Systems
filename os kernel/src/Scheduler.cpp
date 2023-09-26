//
// Created by os on 6/25/22.
//

#include "../h/Scheduler.h"

List<PCB> Scheduler::readyThreadsQueue;

PCB *Scheduler::get() {
    return readyThreadsQueue.removeFirst();
}

void Scheduler::put(PCB *thread) {
    readyThreadsQueue.addLast(thread);
}

