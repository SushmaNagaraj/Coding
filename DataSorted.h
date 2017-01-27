/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef DATASORTED_H_
#define DATASORTED_H_

#include "DataGenerator.h"

/**
 * Generator for already sorted data.
 */
class DataSorted: public DataGenerator
{
public:
    DataSorted() {}
    virtual ~DataSorted() {}

    /**
     * Get the name of this data generator.
     * @return the name.
     */
    string name() { return "Already sorted"; }

    /**
     * Generate already sorted values (low to high) to fill a vector.
     * @param data the data vector to fill.
     */
    virtual void generate_data(vector<int>& data, int size)
    {
        for (int i = 0; i < size; i++) data.push_back(i);
    }
};

#endif /* DATASORTED_H_ */