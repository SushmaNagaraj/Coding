#ifndef DATAALLZEROS_H_
#define DATAALLZEROS_H_

#include "DataGenerator.h"

/**
 * Generator for data that is all zeros.
 */
class DataAllZeros: public DataGenerator
{
public:
    DataAllZeros() {}
    virtual ~DataAllZeros() {}

    /**
     * Get the name of this data generator.
     * @return the name.
     */
    string name() { return "All zeros"; }

    /**
     * Generate all zeros to fill a data vector.
     * @param data the data vector to fill.
     */
    virtual void generate_data(vector<int>& data, int size)
    {
        for (int i = 0; i < size; i++) data.push_back(0);
    }
};

#endif /* DATAALLZEROS_H_ */