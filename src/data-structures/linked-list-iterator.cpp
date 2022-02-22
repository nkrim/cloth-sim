/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Linked List Iterator
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: linked-list-iterator.cpp
 *  Definition file for the LinkedList's iterators.
 * **************************************************************************** */

#ifndef LINKED_LIST_ITER_CPP
#define LINKED_LIST_ITER_CPP

#include "linked-list-iterator.h"

/* ============================================================================ *
 * iterator
 * ============================================================================ */
template<class T>
LLIterator<T>::LLIterator()
    :_node(nullptr) 
{ }

template<class T>
LLIterator<T>::LLIterator(LLNode<T> *node)
    :_node(node) 
{ }

template<class T>
LLIterator<T>::LLIterator(const LLIterator<T>& o)
    :_node(o._node)
{ }

template<class T>
LLIterator<T>& LLIterator<T>::operator++() { 
    _node = _node->next;
    return *this;
}

template<class T>
LLIterator<T> LLIterator<T>::operator++(int) {
    LLIterator ret{*this};
    _node = _node->next;
    return ret;
}

template<class T>
LLIterator<T>& LLIterator<T>::operator--() {
    _node = _node->prev;
    return *this;
}

template<class T>
LLIterator<T> LLIterator<T>::operator--(int) {
    LLIterator ret{*this};
    _node = _node->prev;
    return ret;
}

template<class T>
T& LLIterator<T>::operator*() {
    return _node->val;
}

template<class T>
T* LLIterator<T>::operator->() {
    return &_node->val;
}

template<class T>
bool operator==(const LLIterator<T>& a, const LLIterator<T>& b) {
    return a._node == b._node;
}

template<class T>
bool operator!=(const LLIterator<T>& a, const LLIterator<T>& b) {
    return a._node != b._node;
}

template<class T>
LLIterator<T>::operator RLLIterator<T>() const {
    return RLLIterator<T>(_node);
}

template<class T>
LLIterator<T>::operator CLLIterator<T>() const {
    return CLLIterator<T>(_node);
}



/* ============================================================================ *
 * reverse_iterator
 * ============================================================================ */
template<class T>
RLLIterator<T>::RLLIterator()
    :_node(nullptr) 
{ }

template<class T>
RLLIterator<T>::RLLIterator(LLNode<T> *node)
    :_node(node) 
{ }

template<class T>
RLLIterator<T>::RLLIterator(const RLLIterator<T>& o)
    :_node(o._node)
{ }

template<class T>
RLLIterator<T>& RLLIterator<T>::operator++() { 
    _node = _node->prev;
    return *this;
}

template<class T>
RLLIterator<T> RLLIterator<T>::operator++(int) {
    RLLIterator ret{*this};
    _node = _node->prev;
    return ret;
}

template<class T>
RLLIterator<T>& RLLIterator<T>::operator--() {
    _node = _node->next;
    return *this;
}

template<class T>
RLLIterator<T> RLLIterator<T>::operator--(int) {
    RLLIterator ret{*this};
    _node = _node->next;
    return ret;
}

template<class T>
T& RLLIterator<T>::operator*() {
    return _node->val;
}

template<class T>
T* RLLIterator<T>::operator->() {
    return &_node->val;
}

template<class T>
bool operator==(const RLLIterator<T>& a, const RLLIterator<T>& b) {
    return a._node == b._node;
}

template<class T>
bool operator!=(const RLLIterator<T>& a, const RLLIterator<T>& b) {
    return a._node != b._node;
}

template<class T>
RLLIterator<T>::operator CRLLIterator<T>() const {
    return CRLLIterator<T>(_node);
}



/* ============================================================================ *
 * const_iterator
 * ============================================================================ */
template<class T>
CLLIterator<T>::CLLIterator()
    :_node(nullptr)
{ }

template<class T>
CLLIterator<T>::CLLIterator(const LLNode<T> * const node)
    :_node(node)
{ }

template<class T>
CLLIterator<T>::CLLIterator(const CLLIterator<T>& o)
    :_node(o._node)
{ }

template<class T>
CLLIterator<T>& CLLIterator<T>::operator++() { 
    _node = _node->next;
    return *this;
}

template<class T>
CLLIterator<T> CLLIterator<T>::operator++(int) {
    CLLIterator ret{*this};
    _node = _node->next;
    return ret;
}

template<class T>
CLLIterator<T>& CLLIterator<T>::operator--() {
    _node = _node->prev;
    return *this;
}

template<class T>
CLLIterator<T> CLLIterator<T>::operator--(int) {
    CLLIterator ret{*this};
    _node = _node->prev;
    return ret;
}

template<class T>
const T& CLLIterator<T>::operator*() {
    return _node->val;
}

template<class T>
const T* CLLIterator<T>::operator->() {
    return &_node->val;
}

template<class T>
bool operator==(const CLLIterator<T>& a, const CLLIterator<T>& b) {
    return a._node == b._node;
}

template<class T>
bool operator!=(const CLLIterator<T>& a, const CLLIterator<T>& b) {
    return a._node != b._node;
}



/* ============================================================================ *
 * const_reverse_iterator
 * ============================================================================ */
template<class T>
CRLLIterator<T>::CRLLIterator()
    :_node(nullptr)
{ }

template<class T>
CRLLIterator<T>::CRLLIterator(const LLNode<T> * const node)
    :_node(node)
{ }

template<class T>
CRLLIterator<T>::CRLLIterator(const CRLLIterator<T>& o)
    :_node(o._node)
{ }

template<class T>
CRLLIterator<T>& CRLLIterator<T>::operator++() { 
    _node = _node->prev;
    return *this;
}

template<class T>
CRLLIterator<T> CRLLIterator<T>::operator++(int) {
    CRLLIterator ret{*this};
    _node = _node->prev;
    return ret;
}

template<class T>
CRLLIterator<T>& CRLLIterator<T>::operator--() {
    _node = _node->next;
    return *this;
}

template<class T>
CRLLIterator<T> CRLLIterator<T>::operator--(int) {
    CRLLIterator ret{*this};
    _node = _node->next;
    return ret;
}

template<class T>
const T& CRLLIterator<T>::operator*() {
    return _node->val;
}

template<class T>
const T* CRLLIterator<T>::operator->() {
    return &_node->val;
}

template<class T>
bool operator==(const CRLLIterator<T>& a, const CRLLIterator<T>& b) {
    return a._node == b._node;
}

template<class T>
bool operator!=(const CRLLIterator<T>& a, const CRLLIterator<T>& b) {
    return a._node != b._node;
}



#endif