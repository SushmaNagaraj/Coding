/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef NODE_H_
#define NODE_H_

/**
 * A node of the linked list for mergesort.
 */
class Node
{
public:
    Node() : value(0), next(nullptr) {}
    Node(int value) : value(value), next(nullptr) {}
    virtual ~Node() {};

    int value;
    Node *next;
};

#endif /* NODE_H_ */