/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "Node.h"

/**
 * The linked list class for mergesort.
 */
class LinkedList
{
public:
    LinkedList();
    LinkedList(Node *head, Node *tail, const int size);
    virtual ~LinkedList();

    Node *get_head() const;
    Node *get_tail() const;
    int   get_size() const;

    void set(Node *head, Node *tail, int size);
    Node *remove_head();

    void add(int value);
    void add(Node *node);

    void clear();  // set head and tail to nullptr and delete all the nodes
    void reset();  // set head and tail to nullptr but don't delete the nodes
    void print() const;

    void split(LinkedList& list1, LinkedList& list2);
    void concatenate(LinkedList& other_list);

private:
    Node *head;
    Node *tail;
    int size;
};

#endif /* LINKEDLIST_H_ */