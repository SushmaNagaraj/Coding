/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#include "InsertionSort.h"

/**
 * Default constructor.
 */
InsertionSort::InsertionSort() : VectorSorter() {}

/**
 * Destructor.
 */
InsertionSort::~InsertionSort() {}

/**
 * Get the name of this sorting algorithm.
 * @return the name.
 */
string InsertionSort::name() const { return "Insertion sort"; }

/**
 * Run the insertion sort algorithm.
 * @throws an exception if an error occurred.
 */
 /*  referred from url : https://www.tutorialspoint.com/data_structures_algorithms/insertion_sort_algorithm.htm*/

void InsertionSort::run_sort_algorithm() throw (string)
{
    /***** Complete this member function. *****/
	int i,j;
    for (i = 1; i <size; i++) {
	    int	value = data[i];
        j = i;
		compare_count++;
        while (j > 0 && data[j - 1] >value) {
            compare_count++;
			data[j]=data[j-1];
			move_count++;
            j--;
        }
		if(j!=i){
			data[j]=value;
		    move_count++;
		}

    }
}


