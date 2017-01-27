/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_

#include "VectorSorter.h"

/**
 * The class that implements the insertion sort algorithm
 * for a vector of data.
 */
class InsertionSort: public VectorSorter
{
public:
    InsertionSort();
    virtual ~InsertionSort();

    virtual string name() const;
    virtual void run_sort_algorithm() throw (string);
};

#endif /* INSERTIONSORT_H_ */