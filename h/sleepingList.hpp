//
// Created by os on 5/22/24.
//

#ifndef PROJECT_BASE_V1_1_SLEEPINGLIST_HPP
#define PROJECT_BASE_V1_1_SLEEPINGLIST_HPP

#include "../h/tcb.hpp"
#include "../h/memoryAllocator.hpp"

class SleepingList {
public:
    struct Elem {
        TCB *data;
        Elem *next;
        time_t relativeTime;

        Elem(TCB *data, Elem *next, time_t relativeTime) : data(data), next(next), relativeTime(relativeTime) {}
    };

    Elem *head, *tail;

public:
    SleepingList() : head(nullptr), tail(nullptr) {}

    SleepingList(const SleepingList &) = delete;

    SleepingList &operator=(const SleepingList &) = delete;

    void add(TCB* data, time_t time) {
        if (head == nullptr) {
            head = new Elem(data, nullptr, time);
            tail = head;
            return;
        } else if (time < head->relativeTime) {
            head = new Elem(data, head,time);
            head->next->relativeTime -= time;
            return;
        }
        for (Elem *curr = head; curr->next; curr = curr->next) {
            time -= curr->relativeTime;
            if (time < curr->next->relativeTime) {
                curr->next = new Elem(data, curr->next,time);
                curr->next->next->relativeTime -= time;
                return;
            }
        }
        Elem* tmp = new Elem(data, nullptr,time);
        tail->next = tmp;
        tail = tmp;
    }

    void remove() {
        while (head != nullptr && head->relativeTime == 0){
            Elem *elem = head;
            head = head->next;
            if (!head) { tail = nullptr; }
            elem->data->setSleep(false);
            elem->data->setBlocked(false);
            elem->data->retSemWait = -2;
            Scheduler::put(elem->data);
            delete elem;
        }
    }

    time_t peekTime() {
        if (!head) { return 0; }
        return head->relativeTime;
    }

    void update() {
        if (!head)
            return;
        head->relativeTime--;
    }

    bool isEmpty(){
        if (head == nullptr)
            return true;
        else
            return false;
    }

    void pisi(){
        printInt(tail->data->id_get());
        putc('-');
        printInt(tail->relativeTime);
        printStr("\n");
    }

    void removeThread(TCB* t){
        Elem* tmp = head, *prev = nullptr;
        while(tmp){
            if(tmp->data == t){
                if(prev){
                    prev->next = tmp->next;
                    tmp->next = nullptr;
                }else{
                    head = head->next;
                    if (!head) { tail = nullptr; }
                }
                delete tmp;
                return;
            }
            prev = tmp;
            tmp = tmp->next;
        }
    }

    TCB *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        TCB *ret = elem->data;
        delete elem;
        return ret;
    }
};


#endif //PROJECT_BASE_V1_1_SLEEPINGLIST_HPP
