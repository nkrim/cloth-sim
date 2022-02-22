/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Linked List Iterator
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: linked-list.h
 *  Definition file for templated doubly linked list.
 * **************************************************************************** */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <string>

// linked list node definition
#include "linked-list-node.h"

// forward declare iterators (header included after class declaration)
template<class T> class LLIterator;
template<class T> class RLLIterator;
template<class T> class CLLIterator;
template<class T> class CRLLIterator;

// linked list declaration
template<class T>
class LinkedList {
public:
    // public typedefs
    typedef LLIterator<T> iterator;
    typedef RLLIterator<T> reverse_iterator;
    typedef CLLIterator<T> const_iterator;
    typedef CRLLIterator<T> const_reverse_iterator;

private:
    // member variables
    LLNode<T> *_head;
    LLNode<T> *_tail;
    unsigned _size;

public:
    // constructors
    LinkedList();
    LinkedList(const LinkedList<T>& o);
    LinkedList(LinkedList<T>&& o);
    LinkedList(std::initializer_list<T> il);

    // destructor
    virtual ~LinkedList();

    // assignment
    LinkedList<T>& operator=(const LinkedList<T>& o);
    LinkedList<T>& operator=(LinkedList<T>&& o);

    // insertion
    void insert_head(const T& value);
    void insert_tail(const T& value);
    LinkedList& operator+=(const T& value);
    iterator insert(iterator pos, const T& value);
    reverse_iterator insert(reverse_iterator pos, const T& value);
    bool insert_before(const T& before_this, const T& value);
    bool insert_after(const T& after_this, const T& value);

    // deletion
    bool remove_head();
    bool remove_tail();
    bool remove(const T& value);
    iterator remove(iterator pos, unsigned count=1);
    iterator remove(iterator first, iterator last_exclusive);
    reverse_iterator remove(reverse_iterator pos, unsigned count=1);
    reverse_iterator remove(reverse_iterator first, reverse_iterator last_exclusive);
    unsigned remove_all(const T& value);
    void clear();

    // lookup - unsafe
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // lookup - safe
    T& at(unsigned index);              // can throw std::out_of_range
    const T& at(unsigned index) const;  // can throw std::out_of_range
    iterator iter_at(unsigned index);
    const_iterator iter_at(unsigned index) const;
    iterator find(const T& value);
    const_iterator find(const T& value) const;
    iterator find(const T& value, iterator start);
    const_iterator find(const T& value, const_iterator start) const;
    bool contains(const T& value) const;
    unsigned count(const T& value) const;

    // properties
    unsigned size() const;
    bool empty() const;

    // copy
    LinkedList<T> copy(iterator first, iterator last_exclusive);
    LinkedList<T> copy(reverse_iterator first, reverse_iterator last_exclusive);
    LinkedList<T> copy(const_iterator first, const_iterator last_exclusive);
    LinkedList<T> copy(const_reverse_iterator first, const_reverse_iterator last_exclusive);
    LinkedList<T> reverse_copy(iterator first, iterator last_exclusive);
    LinkedList<T> reverse_copy(reverse_iterator first, reverse_iterator last_exclusive);
    LinkedList<T> reverse_copy(const_iterator first, const_iterator last_exclusive);
    LinkedList<T> reverse_copy(const_reverse_iterator first, const_reverse_iterator last_exclusive);

    // io
    std::string to_string() const;
    template<class U>
    friend std::ostream& operator<<(std::ostream& os, const LinkedList<U>& list);

    // iteration (defined and included from source file)
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

    const_iterator cbegin() const;
    const_iterator cend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    // special iterator positions
    iterator tail_iter();
    const_iterator tail_iter() const;
    reverse_iterator head_iter();
    const_reverse_iterator head_iter() const;

private:
    // private functions
    // insertion
    LLNode<T>* insert_before_node(LLNode<T> *f, const T& value);
    LLNode<T>* insert_after_node(LLNode<T> *f, const T& value);
    // deletion
    bool remove_node(LLNode<T> *f);
    // lookup
    LLNode<T>* find_node(const T& value, LLNode<T>* start=nullptr);
    const LLNode<T>* find_node(const T& value, const LLNode<T>* start=nullptr) const;
};

// iterator includes (has to be after due to friend-class collision)
#include "linked-list-iterator.h"

// class definition includes (for proper templating)
#include "linked-list.cpp"

#endif