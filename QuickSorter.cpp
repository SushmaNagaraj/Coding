/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#include "QuickSorter.h"

/**
 * Default constructor.
 */
QuickSorter::QuickSorter() : VectorSorter() {}

/**
 * Destructor.
 */
QuickSorter::~QuickSorter() {}

/**
 * Run the quicksort algorithm.
 * @throws an exception if an error occurred.
 */
void QuickSorter::run_sort_algorithm() throw (string)
{
    quicksort(0, size-1);
}

/**
 * The quicksort algorithm recursively sorts data subranges.
 * @param left the left index of the subrange to sort.
 * @param right the right index of the subrange to sort.
 */

 /*referred from url: https://www.tutorialspoint.com/data_structures_algorithms/quick_sort_program_in_c.htm
 http://www.java2s.com/Code/Java/Collections-Data-Structure/Quicksortwithmedianofthreepartitioning.htm*/

void QuickSorter::quicksort(const int left, const int right)
{
    /***** Complete this member function. *****/
	if(right-left <= 0) {
      //return;   
   } else {
      int pivot = choose_pivot(left,right);
      int partitionPoint = partition(left, right, pivot);
      quicksort(left,partitionPoint-1);
      quicksort(partitionPoint+1,right);
   } 

}

/**
 * Choose the pivot according to a pivot strategy.
 * The chosen pivot will be moved temporarily to the right end.
 * @param left the left index of the partition to sort.
 * @param right the right index of the partition to sort.
 * @return the pivot value.
 */
int QuickSorter::choose_pivot(const int left, const int right)
{
    return choose_pivot_strategy(left, right);
}

/**
 * Partition the subrange by moving data elements < pivot to the left
 * and data elements > pivot to the right.
 * @param left the left index of the partition to sort.
 * @param right the right index of the partition to sort.
 * @param pivot the pivot value.
 */

  /*referred from url: https://www.tutorialspoint.com/data_structures_algorithms/quick_sort_program_in_c.htm
 http://www.java2s.com/Code/Java/Collections-Data-Structure/Quicksortwithmedianofthreepartitioning.htm*/

int QuickSorter::partition(const int left, const int right, const int pivot)
{
    /***** Complete this member function. *****/
   int leftPointer = left -1;
   int rightPointer = right;

   while(true) {
     while(data[++leftPointer] < pivot) {
         //do nothing
		   compare_count++;
		    compare_count++;
      }
		
      while(rightPointer > 0 && data[--rightPointer] > pivot) {
      compare_count++;
       compare_count++;
      }

      if(leftPointer >= rightPointer) {
         break;
      } else {
         swap(leftPointer,rightPointer);
      }
   }
    swap(leftPointer,right);
   return leftPointer;
}
