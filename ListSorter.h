/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef LISTSORTER_H_
#define LISTSORTER_H_

#include <string>
#include "Sorter.h"
#include "LinkedList.h"

/**
 * Abstract base class of the singly linked list sorting subclasses.
 */
class ListSorter: public Sorter
{
public:
    ListSorter();
    virtual ~ListSorter();

    virtual void generate_data(DataGenerator *generator, int size);
    void print_data() const;
    void clear_data();

protected:
    LinkedList data;  // the data to sort

    bool is_data_sorted() const;
};

#endif /* LISTSORTER_H_ */