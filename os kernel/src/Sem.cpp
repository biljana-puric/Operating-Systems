#include "../h/Sem.h"

int Sem::wait(Sem* pid) {
    PCB* oldRunning = PCB::running;
    if(--(pid->val)<0){
        if (!oldRunning->isFinished()) {
            pid->addLast(oldRunning);
        }
        PCB::running = Scheduler::get();
        PCB::contextSwitch(oldRunning->getContext(), PCB::running->getContext());
    }
    return 0;
}

int Sem::signal(Sem* pid) {
    if(++(pid->val)<=0){
        Scheduler::put(pid->getFirst());
    }
    return 0;
}

int Sem::sem_open(Sem** handle, unsigned int init) {
    Sem* semaphore = new Sem(init);
    *handle = semaphore;
    return 0;
}

int Sem::sem_close(Sem* handle) {
    for(Elem* curr = handle->head; curr!=nullptr; curr = curr->next){
        Scheduler::put(curr->data);
    }
    delete handle;
    return 0;
}