//
// Created by os on 5/12/24.
//

#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}


void operator delete[](void *p) noexcept
{
    mem_free(p);
}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle = nullptr;
    this->body = body;
    this->arg = arg;
}

Thread::~Thread() {
    myHandle->setFinished(true);
    delete myHandle;
}

int Thread::start() {
    if(!myHandle) thread_create(&myHandle, body, arg);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

void Thread::send(char* message){
    ::send(this->myHandle, message);
}

char *Thread::recieve() {
    return ::recieve();
}

Thread::Thread() {
    myHandle = nullptr;
    body = Thread::threadWrapper;
    arg = this;
}

void Thread::threadWrapper(void *thread) {
    ((Thread*)thread)->run();
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
   return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t t) {
    return sem_timedwait(myHandle, t);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

void PeriodicThread::terminate() {
    thread_exit();
}

struct PThreadData
{
    PeriodicThread* thread;
    time_t time;
    PThreadData(PeriodicThread* t, time_t tm) : thread(t), time(tm) {}
};

PeriodicThread::PeriodicThread(time_t period) : Thread(&PeriodicThread::periodicWrapper,new PThreadData(this,period)){

}

void PeriodicThread::periodicWrapper(void *arg)
{
    PThreadData* data = (PThreadData*)arg;
    PeriodicThread* thread = data->thread;
    time_t time = data->time;
    while(true)
    {
        thread->periodicActivation();
        Thread::sleep(time);
    }
}

void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}