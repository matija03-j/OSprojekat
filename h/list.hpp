//
// Created by os on 5/8/24.
//

#ifndef PROJECT_BASE_V1_1_LIST_HPP
#define PROJECT_BASE_V1_1_LIST_HPP

#include "../h/printing.hpp"

template<typename T>
class List
{
public:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;

public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

    int size(){
        int a = 0;
        for (Elem* i = head;  i != nullptr ; i = i->next) {
            a++;
        }
        return a;
    }

    Elem* pisi_id(){
        return head;
    }

    void removeData(T* t){
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

};

#endif //PROJECT_BASE_V1_1_LIST_HPP
