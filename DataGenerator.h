/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef DATAGENERATOR_H_
#define DATAGENERATOR_H_

#include <string>
#include <vector>
using namespace std;

/**
 * Abstract base class of the data generator subclasses.
 */
class DataGenerator
{
public:
    DataGenerator() {}
    virtual ~DataGenerator() {}

    virtual string name() = 0;
    virtual void generate_data(vector<int>& data, int size) = 0;
};

#endif /* DATAGENERATOR_H_ */