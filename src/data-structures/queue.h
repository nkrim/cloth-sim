/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Stack and Queue Templates
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: queue.h
 *  Header file for Queue class template.
 * **************************************************************************** */

/* ---------------------------------------------------------------------------- *
 * NOTE: For all methods, to ensure speed and prevent redundancy, no checks
 *  are made to prevent errors. `empty()` checks should be performed by the 
 *  implementer using the Stack class in order to prevent undefined behavior.
 * ---------------------------------------------------------------------------- */

#ifndef QUEUE_H
#define QUEUE_H

#include "linked-list.h"

#include <iostream>

template<class T>
class Queue {
private:
    // private member variables
    LinkedList<T> _list;

public:
    // constructors
    Queue();
    Queue(std::initializer_list<T> il);

    // container accessors
    T& front();
    const T& front() const;
    T& seek(unsigned n);
    const T& seek(unsigned n) const;

    // container manipulation
    void push(const T& value);
    bool pop();

    // container properties
    bool empty() const;
    unsigned size() const;

    // io
    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const Queue<U>& st);
};

// template definition include
#include "queue.cpp"

#endif