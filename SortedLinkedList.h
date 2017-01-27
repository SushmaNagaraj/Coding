#ifndef SORTEDLINKEDLIST_H_
#define SORTEDLINKEDLIST_H_

/***** Convert this to a class template to handle
 ***** multiple types of data stored in the nodes. */

#include <iostream>
#include "Node.h"

using namespace std;

/**
 * A sorted linked list.
 */
template <class T>
class SortedLinkedList
{
public:
    SortedLinkedList();
    ~SortedLinkedList();

    Node<T> *get_head() const;

    Node<T> *find(T value) const;
    Node<T> *insert(T value);
    void remove(T value);
    void clear();

private:
    Node<T> *head;
};

/**
 * Default constructor.
 */
 template <class T>
SortedLinkedList<T>::SortedLinkedList() : head(nullptr)
{
}

/**
 * Destructor.
 */
 template <class T>
SortedLinkedList<T>::~SortedLinkedList()
{
    clear();
}

/**
 * @return the head.
 */
 template <class T>
Node<T> *SortedLinkedList<T>::get_head() const { return head; }

/**
 * Find a node with a given value in the sorted list.
 * @param value the value of the node to find.
 * @return a pointer to the node if found, else nullptr.
 */
 template <class T>
Node<T> *SortedLinkedList<T>::find(T value) const
{
    Node<T> *p = head;

    // Search the sorted list.
    while ((p != nullptr) && (value > p->data)) p = p->next;

    if ((p != nullptr) && (value == p->data)) return p;        // found
    else                                      return nullptr;  // not found
}

/**
 * Insert a node with a given value into the sorted list.
 * @param value the value for the node to insert.
 * @return a pointer to the inserted node.
 */
 template <class T>
Node<T> *SortedLinkedList<T>::insert(T value)
{
    Node<T> *p = head;
    Node<T> *prev = p;
    Node<T> *new_node = new Node<T>(value);

    // Are we inserting the first node?
    if (head == nullptr)
    {
        head = new_node;
        return new_node;
    }

    // Will the insertion be the new head?
    else if (value < head->data)
    {
        new_node->next = head;
        head = new_node;
        return new_node;
    }

    // Look for where to insert in the sorted list.
    while ((p != nullptr) && (value >= p->data))
    {
        prev = p;
        p = p->next;
    }

    // Insert the node.
    prev->next = new_node;
    new_node->next = p;

    return new_node;
}
template <class T>
void SortedLinkedList<T>::remove(T value)
{
    Node<T> *p = head;
    Node<T> *prev = p;

    if (head == nullptr) return;

    // Is the value at the head?
    if (value == head->data)
    {
        Node<T> *next = head->next;
        delete head;
        head = next;
        return;
    }

    // Look for the node in the sorted list.
    while ((p != nullptr) && (value > p->data))
    {
        prev = p;
        p = p->next;
    }

    // If found, remove the node.
    if ((p != nullptr) && (value == p->data))
    {
        Node<T> *next = p->next;
        delete p;
        prev->next = next;
    }
}

/**
 * Clear the list of all nodes.
 */
 template <class T>
void SortedLinkedList<T>::clear()
{
    // Repeatedly delete the head node until the list is empty.
    while (head != nullptr)
    {
        Node<T> *next = head->next;
        delete head;
        head = next;
    }
}

#endif /* SORTEDLINKEDLIST_H_ */
