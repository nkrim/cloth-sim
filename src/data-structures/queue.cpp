/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Stack and Queue Templates
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: queue.cpp
 *  Definition file for Queue class template.
 * **************************************************************************** */

/* ---------------------------------------------------------------------------- *
 * NOTE: For all methods, to ensure speed and prevent redundancy, no checks
 *  are made to prevent errors. `empty()` checks should be performed by the 
 *  implementer using the Queue class in order to prevent undefined behavior.
 * ---------------------------------------------------------------------------- */

#ifndef QUEUE_CPP
#define QUEUE_CPP

#include "queue.h"

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
template<class T>
Queue<T>::Queue()
    : _list{}
{ }

template<class T>
Queue<T>::Queue(std::initializer_list<T> il)
    : _list{}
{
    for(auto it=il.begin(); it!=il.end(); ++it)
        _list.insert_tail(*it);
}

/* ============================================================================ *
 * Container Accessors
 * ============================================================================ */
template<class T>
T& Queue<T>::front() {
    return _list.front();
}

template<class T>
const T& Queue<T>::front() const {
    return _list.front();
}

template<class T>
T& Queue<T>::seek(unsigned n) {
    return _list.at(n);
}

template<class T>
const T& Queue<T>::seek(unsigned n) const {
    return _list.at(n);
}

/* ============================================================================ *
 * Container Manipulation
 * ============================================================================ */
template<class T>
void Queue<T>::push(const T& value) {
    _list.insert_tail(value);
}

template<class T>
bool Queue<T>::pop() {
    return _list.remove_head();
}

/* ============================================================================ *
 * Container Properties
 * ============================================================================ */
template<class T>
bool Queue<T>::empty() const {
    return _list.empty();
}

template<class T>
unsigned Queue<T>::size() const {
    return _list.size();
}

// io
template<class T>
std::ostream& operator<<(std::ostream& os, const Queue<T>& st) {
    return os << "{Front:" << st._list << '}';
}

#endif