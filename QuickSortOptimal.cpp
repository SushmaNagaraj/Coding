/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#include "QuickSortOptimal.h"

/**
 * Get the name of this sorting algorithm.
 * @return the name.
 */
QuickSortOptimal::QuickSortOptimal() {}

/**
 * Destructor.
 */
QuickSortOptimal::~QuickSortOptimal() {}

/**
 * Get the name of this sorting algorithm.
 * @return the name.
 */
string QuickSortOptimal::name() const { return "Quicksort optimal"; }

/**
 * Choose a good pivot, the median-of-three:
 * The middle value of the leftmost, rightmost, and center elements.
 * This is a compromise since the most optimal pivot would be the
 * median of the subrange, but that's too expensive to compute.
 * @param left the left index of the subrange to sort.
 * @param right the right index of the subrange to sort.
 * @return the chosen pivot value.
 */
  /*referred from url: https://www.tutorialspoint.com/data_structures_algorithms/quick_sort_program_in_c.htm
 http://www.java2s.com/Code/Java/Collections-Data-Structure/Quicksortwithmedianofthreepartitioning.htm*/

int QuickSortOptimal::choose_pivot_strategy(const int left, const int right)
{
    /***** Complete this member function. *****/
	int center = (left + right) / 2;
    // order left & center
    if (data[left] > data[center]){
      swap(left, center);
    }
	 // order left & right
	if (data[left] > data[right]){
      swap(left, right);
	}
    // order center & right
    if (data[center] > data[right]){
      swap(center, right);
	}
    compare_count = compare_count+3;
    swap(center, right); // put pivot on right
    return data[right]; // return median value

}
