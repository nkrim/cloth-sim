/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Stack and Queue Templates
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: stack.cpp
 *  Definition file for Stack class template.
 * **************************************************************************** */

/* ---------------------------------------------------------------------------- *
 * NOTE: For all methods, to ensure speed and prevent redundancy, no checks
 *  are made to prevent errors. `empty()` checks should be performed by the 
 *  implementer using the Stack class in order to prevent undefined behavior.
 * ---------------------------------------------------------------------------- */

#ifndef STACK_CPP
#define STACK_CPP

#include "stack.h"

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
template<class T>
Stack<T>::Stack()
    : _list{}
{ }

template<class T>
Stack<T>::Stack(std::initializer_list<T> il)
    : _list{}
{
    for(auto it=il.begin(); it!=il.end(); ++it)
        _list.insert_head(*it);
}

/* ============================================================================ *
 * Container Accessors
 * ============================================================================ */
template<class T>
T& Stack<T>::top() {
    return _list.front();
}

template<class T>
const T& Stack<T>::top() const {
    return _list.front();
}

template<class T>
T& Stack<T>::seek(unsigned n) {
    return _list.at(n);
}

template<class T>
const T& Stack<T>::seek(unsigned n) const {
    return _list.at(n);
}

/* ============================================================================ *
 * Container Manipulation
 * ============================================================================ */
template<class T>
void Stack<T>::push(const T& value) {
    _list.insert_head(value);
}

template<class T>
bool Stack<T>::pop() {
    return _list.remove_head();
}

/* ============================================================================ *
 * Container Properties
 * ============================================================================ */
template<class T>
bool Stack<T>::empty() const {
    return _list.empty();
}

template<class T>
unsigned Stack<T>::size() const {
    return _list.size();
}

// io
template<class T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& st) {
    return os << "{Top:" << st._list << '}';
}

#endif