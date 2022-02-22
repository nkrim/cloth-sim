/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  Linked List Iterator
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: linked-list0node.h
 *  Header file for LinkedList's nodes
 * **************************************************************************** */

#ifndef LINKED_LIST_NODE_H
#define LINKED_LIST_NODE_H

template<class T>
struct LLNode {
    T val;
    LLNode<T> *prev;
    LLNode<T> *next;

    LLNode(T v):val(v), prev(nullptr), next(nullptr) {}
};

#endif