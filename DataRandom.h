/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#ifndef DATARANDOM_H_
#define DATARANDOM_H_

#include <cstdlib>
#include <ctime>
#include "DataGenerator.h"

/**
 * Generator for unsorted random data.
 */
class DataRandom: public DataGenerator
{
public:
    DataRandom() {}
    virtual ~DataRandom() {}

    /**
     * Get the name of this data generator.
     * @return the name.
     */
    string name() { return "Unsorted random"; }

    vector<int> random_data;

    /**
     * Generate random values to fill a data vector.
     * Reuse previously generated data if possible.
     * @param data the target data vector.
     * @param size the number of random data elements to generate.
     */
    virtual void generate_data(vector<int>& data, int size)
    {
        data.clear();

        // Reuse previously generated random data.
        if (random_data.size() == size)
        {
            int size = random_data.size();
            for (int i = 0; i < size; i++) data.push_back(random_data[i]);
        }

        // Generate new random data.
        else
        {
            random_data.clear();
            srand(time(NULL));  // seed the random number generator

            for (int i = 0; i < size; i++) {
                int r = rand()%size;
                random_data.push_back(r);
                data.push_back(r);
            }
        }
    }
};

#endif /* DATARANDOM_H_ */