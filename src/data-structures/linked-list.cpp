/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Linked List Iterator
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: linked-list.h
 *  Header file for templated doubly linked list.
 * **************************************************************************** */

#ifndef LINKED_LIST_CPP
#define LINKED_LIST_CPP

#include "linked-list.h"

#include <sstream>

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
template<class T>
LinkedList<T>::LinkedList()
    : _head(nullptr)
    , _tail(nullptr)
    , _size(0)
{ }

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& o)
    :LinkedList{}
{ 
    *this = o;
}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T>&& o)
    :LinkedList{}
{
    *this = std::move(o);
}

template<class T>
LinkedList<T>::LinkedList(const std::initializer_list<T> il)
    :LinkedList{}
{
    for(auto it=il.begin(); it!=il.end(); ++it)
        insert_tail(*it);
}


/* ============================================================================ *
 * Destructor
 * ============================================================================ */
template<class T>
LinkedList<T>::~LinkedList() {
    clear();    // clear() function deletes all nodes
}


/* ============================================================================ *
 * Assignment
 * ============================================================================ */
template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& o) {
    // clear this list
    this->clear();

    // copy from o (_size adjusted from insert_tail function)
    for(LLNode<T> *curr=o._head; curr!=nullptr; curr=curr->next)
        this->insert_tail(curr->val);

    // return
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& o) {
    // clear this list
    this->clear();

    // move from o
    this->_head = o._head;
    this->_tail = o._tail;
    this->_size = o._size;
    // unset values in o
    o._head = nullptr;
    o._tail = nullptr;
    o._size = 0;
    // return 
    return *this;
}


/* ============================================================================ *
 * Insertion
 * ============================================================================ */
template<class T>
void LinkedList<T>::insert_head(const T& value) {
    // init node to insert
    LLNode<T> *n = new LLNode<T>(value);

    // case of empty list
    if(_head == nullptr) {
        _head = n;
        _tail = n;
    }
    // case of non-null _head 
    else {
        n->next = _head;
        _head->prev = n;
        _head = n;
    }

    // increment size
    ++_size;
}

template<class T>
void LinkedList<T>::insert_tail(const T& value) {
    // init node to insert
    LLNode<T> *n = new LLNode<T>(value);

    // case of empty list
    if(_tail == nullptr) {
        _head = n;
        _tail = n;
    }
    // case of non-null _head 
    else {
        n->prev = _tail;
        _tail->next = n;
        _tail = n;
    }

    // increment size
    ++_size;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator+=(const T& value) {
    insert_tail(value);
    return *this;
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::insert(iterator pos, const T& value) {
    if(pos == end()) {
        insert_tail(value);
        return iterator(_tail);
    }
    return iterator( insert_before_node(pos._node, value) );
}

template<class T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::insert(reverse_iterator pos, const T& value) {
    if(pos == rend()) {
        insert_head(value);
        return reverse_iterator(_head);
    }
    return reverse_iterator( insert_after_node(pos._node, value) );
}

template<class T>
bool LinkedList<T>::insert_before(const T& before_this, const T& value) {
    // insert_before_node() returns a pointer to the node it added, or nullptr if that was passed
    return insert_before_node( find_node(before_this), value ) != nullptr;
}

template<class T>
bool LinkedList<T>::insert_after(const T& after_this, const T& value) {
    // insert_after_node() returns a pointer to the node it added, or nullptr if that was passed
    return insert_after_node( find_node(after_this), value ) != nullptr;
}


/* ============================================================================ *
 * Deletion
 * ============================================================================ */
template<class T>
bool LinkedList<T>::remove_head() {
    return remove_node(_head);
}

template<class T>
bool LinkedList<T>::remove_tail() {
    return remove_node(_tail);
}

template<class T>
bool LinkedList<T>::remove(const T& value) {
    // remove_node() returns false when given nullptr
    return remove_node( find_node(value) );
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::remove(iterator pos, unsigned count) {
    LLNode<T> *curr = pos._node;
    LLNode<T> *next;
    while(count > 0 && curr != nullptr) {
        next = curr->next;
        remove_node(curr);
        curr = next;
        --count;
    }
    return iterator(curr);
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::remove(iterator first, iterator last_exclusive) {
    auto curr = first;
    while(first != last_exclusive) {
        ++first;
        remove_node(curr._node);
        curr = first;
    }
    return last_exclusive;
}

template<class T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::remove(reverse_iterator pos, unsigned count) {
    LLNode<T> *curr = pos._node;
    LLNode<T> *next;
    while(count > 0 && curr != nullptr) {
        next = curr->next;
        remove_node(curr);
        curr = next;
        --count;
    }
    return reverse_iterator(curr);
}

template<class T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::remove(reverse_iterator first, reverse_iterator last_exclusive) {
    auto curr = first;
    while(first != last_exclusive) {
        ++first;
        remove_node(curr._node);
        curr = first;
    }
    return last_exclusive;
}

template<class T>
unsigned LinkedList<T>::remove_all(const T& value) {
    LLNode<T> *f;
    LLNode<T> *s = _head;   // starting node for find_node()
    unsigned count = 0;

    // incrementally search for nodes with given value
    while(s!=nullptr && (f=find_node(value, s))!=nullptr) {
        s = f->next;
        remove_node(f);
        count++;
    }

    // return number of deleted node
    return count;
}

template<class T>
void LinkedList<T>::clear() {
    // if empty, exit early
    if(_head == nullptr)
        return;

    // traverse and delete behind walker
    LLNode<T> *curr = _head;
    while(curr != _tail) {
        curr = curr->next;
        delete curr->prev;
    }
    // delete where walker ended up (will be _tail)
    delete curr;

    // reset member variables
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
}


/* ============================================================================ *
 * Lookup - Unsafe
 * ============================================================================ */
template<class T>
T& LinkedList<T>::front() {
    return _head->val;
}

template<class T>
const T& LinkedList<T>::front() const {
    return _head->val;
}

template<class T>
T& LinkedList<T>::back() {
    return _tail->val;
}

template<class T>
const T& LinkedList<T>::back() const {
    return _tail->val;
}


/* ============================================================================ *
 * Lookup - Safe
 * ============================================================================ */
template<class T>
T& LinkedList<T>::at(unsigned index) {
    if(index >= _size)
        throw std::out_of_range("Stack::at() : out of range.");

    LLNode<T> *curr = _head;
    while(index>0 && curr!=nullptr) {
        --index;
        curr = curr->next;
    }
    if(curr == nullptr)
        throw std::out_of_range("Stack::at() : invalid linked list, size mismatch.");
    
    return curr->val;
}

template<class T>
const T& LinkedList<T>::at(unsigned index) const {
    if(index >= _size)
        throw std::out_of_range("Stack::at() const : out of range.");

    LLNode<T> *curr = _head;
    while(index>0 && curr!=nullptr) {
        --index;
        curr = curr->next;
    }
    if(curr == nullptr)
        throw std::out_of_range("Stack::at() const : invalid linked list, size mismatch.");
    
    return curr->val;
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::iter_at(unsigned index) {
    // point to end if exceeds size
    if(index >= _size) 
        return end();

    LLNode<T> *curr;
    // start from head
    if(index < _size/2) {
        curr = _head;
        while(index > 0 && curr != nullptr) {
            curr = curr->next;
            --index;
        }
    }
    // start from tail
    else {
        curr = _tail;
        while(index < _size-1 && curr != nullptr) {
            curr = curr->prev;
            ++index;
        }
    }
    return iterator(curr);
}

template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::iter_at(unsigned index) const {
    // point to end if exceeds size
    if(index >= _size)
        return end();

    const LLNode<T> *curr;
    // start from head
    if(index < _size/2) {
        curr = _head;
        while(index > 0 && curr != nullptr) {
            curr = curr->next;
            --index;
        }
    }
    // start from tail
    else {
        curr = _tail;
        while(index < _size-1 && curr != nullptr) {
            curr = curr->prev;
            ++index;
        }
    }
    return const_iterator(curr);
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::find(const T& value) {
    return iterator( find_node(value) );
}

template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::find(const T& value) const {
    return const_iterator( find_node(value) );
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::find(const T& value, iterator start) {
    if(start == end())
        return start;
    return iterator( find_node(value, start._node) );
}

template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::find(const T& value, const_iterator start) const {
    if(start == end())
        return start;
    return const_iterator( find_node(value, start._node) );
}

template<class T>
bool LinkedList<T>::contains(const T& value) const {
    return find_node(value) != nullptr;
}

template<class T>
unsigned LinkedList<T>::count(const T& value) const {
    const LLNode<T> *curr = _head;
    unsigned count = 0;

    // incrementally search for nodes with given value
    while(curr!=nullptr && (curr=find_node(value, curr))!=nullptr) {
        count++;
        curr = curr->next;    // set curr to starting point for next search
    }

    // return number of found nodes
    return count;
}


/* ============================================================================ *
 * Properties
 * ============================================================================ */
template<class T>
unsigned LinkedList<T>::size() const {
    return _size;
}

template<class T>
bool LinkedList<T>::empty() const {
    return _head == nullptr;
}


/* ============================================================================ *
 * Copy
 * ============================================================================ */
template<class T>
LinkedList<T> LinkedList<T>::copy(iterator first, iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_tail(*first);
        ++first;
    }
    return c;
}

template<class T>
LinkedList<T> LinkedList<T>::copy(reverse_iterator first, reverse_iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_head(*first);
        ++first;
    }
    return c;
}

template<class T>
LinkedList<T> LinkedList<T>::copy(const_iterator first, const_iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_tail(*first);
        ++first;
    }
    return c;
}

template<class T>
LinkedList<T> LinkedList<T>::copy(const_reverse_iterator first, const_reverse_iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_head(*first);
        ++first;
    }
    return c;
}

template<class T>
LinkedList<T> LinkedList<T>::reverse_copy(iterator first, iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_head(*first);
        ++first;
    }
    return c;
}

template<class T>
LinkedList<T> LinkedList<T>::reverse_copy(reverse_iterator first, reverse_iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_tail(*first);
        ++first;
    }
    return c;
}

template<class T>
LinkedList<T> LinkedList<T>::reverse_copy(const_iterator first, const_iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_head(*first);
        ++first;
    }
    return c;
}

template<class T>
LinkedList<T> LinkedList<T>::reverse_copy(const_reverse_iterator first, const_reverse_iterator last_exclusive) {
    LinkedList<T> c;
    while(first != last_exclusive) {
        c.insert_tail(*first);
        ++first;
    }
    return c;
}


/* ============================================================================ *
 * IO
 * ============================================================================ */
template<class T>
std::string LinkedList<T>::to_string() const {
    // if empty, spit out empty list string
    if(_head == nullptr)
        return "[]";

    // print first node outside of loop
    std::stringstream ss;
    LLNode<T> *curr = _head;
    ss << '[' << curr->val;
    // traverse list, print each value
    while((curr=curr->next) != nullptr)
        ss << " -> " << curr->val;
    // close bracket the list
    ss << ']';

    // return string
    return ss.str();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
    return os << list.to_string();
}


/* ============================================================================ *
 * Iteration
 * ============================================================================ */
template<class T>
typename LinkedList<T>::iterator LinkedList<T>::begin() {
    return iterator(_head);
}
template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::begin() const {
    return const_iterator(_head);
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::end() {
    return iterator();
}
template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::end() const {
    return const_iterator();
}

template<class T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::rbegin() {
    return reverse_iterator(_tail);
}
template<class T>
typename LinkedList<T>::const_reverse_iterator LinkedList<T>::rbegin() const {
    return const_reverse_iterator(_tail);
}

template<class T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::rend() {
    return reverse_iterator();
}
template<class T>
typename LinkedList<T>::const_reverse_iterator LinkedList<T>::rend() const {
    return const_reverse_iterator();
}

template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::cbegin() const {
    return const_iterator(_head);
}

template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::cend() const {
    return const_iterator();
}

template<class T>
typename LinkedList<T>::const_reverse_iterator LinkedList<T>::crbegin() const {
    return const_reverse_iterator(_tail);
}

template<class T>
typename LinkedList<T>::const_reverse_iterator LinkedList<T>::crend() const {
    return const_reverse_iterator(); 
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::tail_iter() {
    return iterator(_tail);
}

template<class T>
typename LinkedList<T>::const_iterator LinkedList<T>::tail_iter() const {
    return const_iterator(_tail);
}

template<class T>
typename LinkedList<T>::reverse_iterator LinkedList<T>::head_iter() {
    return reverse_iterator(_head);
}

template<class T>
typename LinkedList<T>::const_reverse_iterator LinkedList<T>::head_iter() const {
    return const_reverse_iterator(_head);
}



/* ============================================================================ *
 * Private Functions - Insertion
 * ============================================================================ */
template<class T>
LLNode<T>* LinkedList<T>::insert_before_node(LLNode<T> *f, const T& value) {
    // given node is nullptr, exit signalling failure
    if(f == nullptr)
        return nullptr;

    // if f is _head, fallback on insert_head()
    if(f == _head) {
        insert_head(value);
        return _head;
    }

    // insert manually (known that f->prev exists as f!=_head)
    LLNode<T> *n = new LLNode<T>(value);
    n->next = f;        // set n's next to f
    n->prev = f->prev;  // set n's prev to f's prev
    n->prev->next = n;  // set prev's next to n
    f->prev = n;        // set f's prev to n

    // increment size
    ++_size;

    // exit successfully 
    return n;
}

template<class T>
LLNode<T>* LinkedList<T>::insert_after_node(LLNode<T> *f, const T& value) {
    // given node is nullptr, exit signalling failure
    if(f == nullptr)
        return nullptr;

    // if f is _tail, fallback on insert_tail()
    if(f == _tail) {
        insert_tail(value);
        return _tail;
    }

    // insert manually (known that f->next exists as f!=_tail)
    LLNode<T> *n = new LLNode<T>(value);
    n->prev = f;        // set n's prev to f
    n->next = f->next;  // set n's next to f's next
    n->next->prev = n;  // set next's prev to n
    f->next = n;        // set f's next to n

    // increment size
    ++_size;

    // exit successfully 
    return n;
}


/* ============================================================================ *
 * Private Functions - Deletion
 * ============================================================================ */
template<class T>
bool LinkedList<T>::remove_node(LLNode<T> *f) {
    // f is nullptr, exit signalling failure
    if(f == nullptr)
        return false;

    // case : f is last node
    if(_head == _tail) {
        // clear list
        _head = nullptr;
        _tail = nullptr;
    }
    // case : f is _head (known _head->next exists)
    else if(f == _head) {
        // set new _head as _head->next 
        _head = _head->next;
        _head->prev = nullptr;
    }
    // case : f is _tail (known _tail->prev exists)
    else if(f == _tail) {
        // set new _tail as _tail->prev 
        _tail = _tail->prev;
        _tail->next = nullptr;
    }
    // case : f is in the middle
    else {
        // unlink f from list
        f->prev->next = f->next;
        f->next->prev = f->prev;
    }

    // delete node
    delete f;

    // decrement size
    --_size;

    // exit successfully
    return true;
}


/* ============================================================================ *
 * Private Functions - Lookup
 * ============================================================================ */
template<class T>
LLNode<T>* LinkedList<T>::find_node(const T& value, LLNode<T>* start) {
    // if start has default value (nullptr), direct to _head
    if(start == nullptr)
        start = _head;

    // iterate through list to find node with given value
    for(LLNode<T> *curr=start; curr!=nullptr; curr=curr->next) {
        if(curr->val == value)
            return curr;
    }

    // node not found, return nullptr
    return nullptr;
}

template<class T>
const LLNode<T>* LinkedList<T>::find_node(const T& value, const LLNode<T>* start) const {
    // if start has default value (nullptr), direct to _head
    if(start == nullptr)
        start = _head;

    // iterate through list to find node with given value
    for(const LLNode<T> *curr=start; curr!=nullptr; curr=curr->next) {
        if(curr->val == value)
            return curr;
    }

    // node not found, return nullptr
    return nullptr;
}

#endif