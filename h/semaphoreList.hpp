//
// Created by os on 6/15/24.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHORELIST_HPP
#define PROJECT_BASE_V1_1_SEMAPHORELIST_HPP

#include "../h/semaphore.hpp"

class SemaphoreList{
public:
    struct Elem{
        SemaphoreK* sem;
        Elem* next;

        Elem(SemaphoreK *sem, Elem *next) : sem(sem), next(next) {}
    };

    Elem* head, *tail;

    SemaphoreList() : head(nullptr), tail(nullptr) {}
    SemaphoreList(const SemaphoreList &) = delete;
    SemaphoreList &operator=(const SemaphoreList &) = delete;

    void add(SemaphoreK *sem)
    {
        Elem *elem = new Elem(sem, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    bool find(SemaphoreK* sem){
        Elem* tmp = head;
        while(tmp){
            if(tmp->sem == sem){
                return true;
            }
            tmp = tmp->next;
        }
        return false;
    }

    void update(){
        Elem* tmp = head, *prev = nullptr;
        while(tmp){
            tmp->sem->getWaitList()->update();
            tmp->sem->getWaitList()->remove();
            if(tmp->sem->getWaitList()->isEmpty()){
                if(prev){
                    prev->next = tmp->next;
                    tmp->next = nullptr;
                }else{
                    head = head->next;
                    if (!head) { tail = nullptr; }
                }
                Elem* del = tmp;
                tmp = tmp->next;
                delete del;
                continue;
            }
            prev = tmp;
            tmp = tmp->next;
        }
    }


};

#endif //PROJECT_BASE_V1_1_SEMAPHORELIST_HPP
