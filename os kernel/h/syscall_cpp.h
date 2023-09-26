//
// Created by os on 6/23/22.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_H
#define PROJECT_BASE_V1_1_SYSCALL_CPP_H

#include "../h/syscall_c.h"

//void* operator new (size_t size){
//    return mem_alloc(size);
//}
//void operator delete (void* a) noexcept{
//    mem_free(a);
//}

class Thread {
public:
    Thread (void (*body)(void*), void* arg){
        thread_create (&myHandle, body, arg);
    }
    virtual ~Thread () {
        thread_exit();
    }
    int start (){
        if(myHandle){
            run();
        }
        else{
            thread_create(&myHandle, &runWrapper, this);
        }
        return 0;
    }
    static void dispatch (){
        thread_dispatch();
    }
    static int sleep (time_t t){
        int ret = time_sleep(t);
        return ret;
    }
protected:
    Thread (){
        myHandle= nullptr;
    }
    virtual void run () {}
private:
    thread_t myHandle;
    static void runWrapper(void* thread){
        Thread* t = (Thread*) thread;
        t->run();
    }

};

class Semaphore {
public:
    Semaphore (unsigned init = 1){
        sem_open(&myHandle, init);
    }
    virtual ~Semaphore (){
        sem_close(myHandle);
    }
    int wait (){
        return sem_wait(myHandle);
    }
    int signal (){
        return sem_signal(myHandle);
    }
private:
    sem_t myHandle;
};


class PeriodicThread : public Thread {
protected:
    PeriodicThread (time_t period){
        periodicActivation();
        sleep(period);
    }
    virtual void periodicActivation () {}
};

class Console {
public:
    static char getc (){
        return ::getc();
    }
    static void putc (char c){
        ::putc(c);
    }
};

#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_H
