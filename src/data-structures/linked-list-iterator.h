/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Linked List Iterator
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: linked-list-iterator.h
 *  Header file for the LinkedList's iterators.
 * **************************************************************************** */

#ifndef LINKED_LIST_ITER_H
#define LINKED_LIST_ITER_H

#include "linked-list-node.h"
#include "linked-list.h"

// iterator definition
template<class T>
class LLIterator {
    friend class LinkedList<T>;

private:
    LLNode<T> *_node;

public:
    LLIterator();
    LLIterator(LLNode<T> *node);
    LLIterator(const LLIterator& o);
    LLIterator& operator++();
    LLIterator operator++(int);
    LLIterator& operator--();
    LLIterator operator--(int);
    T& operator*();
    T* operator->();
    template<class U>
    friend bool operator==(const LLIterator<U>& a, const LLIterator<U>& b);
    template<class U>
    friend bool operator!=(const LLIterator<U>& a, const LLIterator<U>& b);
    operator RLLIterator<T>() const;
    operator CLLIterator<T>() const;
};

// reverse_iterator definition
template<class T>
class RLLIterator {
    friend class LinkedList<T>;

private:
    LLNode<T> *_node;

public:
    RLLIterator();
    RLLIterator(LLNode<T> *node);
    RLLIterator(const RLLIterator& o);
    RLLIterator& operator++();
    RLLIterator operator++(int);
    RLLIterator& operator--();
    RLLIterator operator--(int);
    T& operator*();
    T* operator->();
    template<class U>
    friend bool operator==(const RLLIterator<U>& a, const RLLIterator<U>& b);
    template<class U>
    friend bool operator!=(const RLLIterator<U>& a, const RLLIterator<U>& b);
    operator CRLLIterator<T>() const;
};

// const_iterator definition
template<class T>
class CLLIterator {
    friend class LinkedList<T>;

private:
    const LLNode<T> *_node;

public:
    CLLIterator();
    CLLIterator(const LLNode<T> * const node);
    CLLIterator(const CLLIterator& o);
    CLLIterator& operator++();
    CLLIterator operator++(int);
    CLLIterator& operator--();
    CLLIterator operator--(int);
    const T& operator*();
    const T* operator->();
    template<class U>
    friend bool operator==(const CLLIterator<U>& a, const CLLIterator<U>& b);
    template<class U>
    friend bool operator!=(const CLLIterator<U>& a, const CLLIterator<U>& b);
};

// const_reverse_iterator definition
template<class T>
class CRLLIterator {
    friend class LinkedList<T>;
    
private:
    const LLNode<T> *_node;

public:
    CRLLIterator();
    CRLLIterator(const LLNode<T> * const node);
    CRLLIterator(const CRLLIterator& o);
    CRLLIterator& operator++();
    CRLLIterator operator++(int);
    CRLLIterator& operator--();
    CRLLIterator operator--(int);
    const T& operator*();
    const T* operator->();
    template<class U>
    friend bool operator==(const CRLLIterator<U>& a, const CRLLIterator<U>& b);
    template<class U>
    friend bool operator!=(const CRLLIterator<U>& a, const CRLLIterator<U>& b);
};

#include "linked-list-iterator.cpp"

#endif 