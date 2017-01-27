/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef QUICKSORTER_H_
#define QUICKSORTER_H_

#include <string>
#include "VectorSorter.h"

/**
 * The abstract base class of the quicksort subclasses.
 */
class QuickSorter: public VectorSorter
{
public:
    QuickSorter();
    QuickSorter(string name);
    virtual ~QuickSorter();

    virtual void run_sort_algorithm() throw (string);

protected:
    virtual int choose_pivot_strategy(const int left, const int right) = 0;

private:
    void quicksort(const int left, const int right);
    int partition(const int left, const int right, const int pivot);
    int choose_pivot(const int left, const int right);
};

#endif /* QUICKSORTER_H_ */