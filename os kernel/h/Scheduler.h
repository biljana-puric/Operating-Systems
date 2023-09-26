//
// Created by os on 6/25/22.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_H
#define PROJECT_BASE_V1_1_SCHEDULER_H

#include "../h/list.h"

class PCB;
class Scheduler {
private:
    static List<PCB> readyThreadsQueue;

public:
    static PCB *get();
    static void put(PCB *thread);
};


#endif //PROJECT_BASE_V1_1_SCHEDULER_H
