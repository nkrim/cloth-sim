/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Stack and Queue Templates
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: stack.h
 *  Header file for Stack class template.
 * **************************************************************************** */

/* ---------------------------------------------------------------------------- *
 * NOTE: For all methods, to ensure speed and prevent redundancy, no checks
 *  are made to prevent errors. `empty()` checks should be performed by the 
 *  implementer using the Stack class in order to prevent undefined behavior.
 * ---------------------------------------------------------------------------- */

#ifndef STACK_H
#define STACK_H

#include "linked-list.h"

#include <iostream>

template<class T>
class Stack {
private:
    // private member variables
    LinkedList<T> _list;

public:
    // constructors
    Stack();
    Stack(std::initializer_list<T> il);

    // container accessors
    T& top();
    const T& top() const;
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
    friend std::ostream& operator<<(std::ostream& os, const Stack<U>& st);
};

// template definition include
#include "stack.cpp"

#endif