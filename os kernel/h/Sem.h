#ifndef PROJECT_BASE_V1_1_SEM_H
#define PROJECT_BASE_V1_1_SEM_H
#include "../h/PCB.h"

class Sem{
public:
    explicit Sem(unsigned int init=1){
        val=init;
    };
    static int wait(Sem* pid);
    static int signal(Sem* pid);
    static int sem_open(Sem** handle, unsigned int init);
    static int sem_close(Sem* handle);
private:
    int val;
    struct Elem{
        PCB* data;
        Elem* next;
        Elem(PCB* pcb, Elem* n) : data(pcb), next(n){}
    };
    Elem* head = nullptr, *tail= nullptr;

    void addLast(PCB* thread){
        if(!head){
            head = new Elem(thread, nullptr);
            tail=head;
        }
        else{
            tail->next = new Elem(thread, nullptr);
            tail = tail->next;
        }
    }

    PCB* getFirst(){
        if (!head) { return nullptr; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = nullptr; }

        PCB *ret = elem->data;
        delete elem;
        return ret;
    }

};


#endif //PROJECT_BASE_V1_1_SEM_H