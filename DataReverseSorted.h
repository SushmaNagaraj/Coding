/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef DATAREVERSESORTED_H_
#define DATAREVERSESORTED_H_

#include "DataGenerator.h"

/**
 * Generator for reverse-sorted data.
 */
class DataReverseSorted: public DataGenerator
{
public:
    DataReverseSorted() {}
    virtual ~DataReverseSorted() {}

    /**
     * Get the name of this data generator.
     * @return the name.
     */
    string name() { return "Reverse sorted"; }

    /**
     * Generate reverse sorted values (high to low) to fill a data vector.
     * @param data the data vector to fill.
     */
    virtual void generate_data(vector<int>& data, int size)
    {
        for (int i = 0; i < size; i++) data.push_back(size - i);
    }
};

#endif /* DATAREVERSESORTED_H_ */