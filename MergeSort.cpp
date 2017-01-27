/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#include <string>
#include "MergeSort.h"

/**
 * Default constructor.
 */
MergeSort::MergeSort() : ListSorter() {}

/**
 * Destructor.
 */
MergeSort::~MergeSort() {}

/**
 * Get the name of this sorting algorithm.
 * @return the name.
 */
string MergeSort::name() const { return "Mergesort"; }

/**
 * Run the mergesort algorithm.
 * @throws an exception if an error occurred.
 */
void MergeSort::run_sort_algorithm()
    throw (string)
{
    // Defensive programming: Make sure we end up
    // with the same size list after sorting.
    int size_before = data.get_size();

    mergesort(data);

    int size_after = data.get_size();
    if (size_before != size_after)
    {
        string message = "***** Size mismatch: before " +
                         to_string(size_before) + ", size after " +
                         to_string(size_after);
        throw message;
    }
}

/**
 * The mergesort algorithm recursively splits and merges data lists.
 * @param list the list of data to sort.
 */
void MergeSort::mergesort(LinkedList& list)
{
    /***** Complete this member function. *****/
	int num = list.get_size();
	LinkedList start;
	LinkedList end;
	if (num < 2)
		return;

	list.split(start, end);
	mergesort(start);
	mergesort(end);
	merge(list,start,end);
}

/**
 * Merge two sublists back into a single list.
 * @param list the merged list.
 * @param sublist1 the first sublist.
 * @param sublist2 the second sublist.
 */
void MergeSort::merge(LinkedList& list,
                      LinkedList& sublist1, LinkedList& sublist2)
{
    /***** Complete this member function. *****/
		list.clear();
	    int startl = sublist1.get_size();
		int endr= sublist2.get_size();

		int i, j, k;
		i =0;
		j=0;
		k=0;


		for (;i<startl && j<endr;)
		{
		    compare_count++;
			if (sublist1.get_head()->value <= sublist2.get_head()->value)
			{
			    move_count++;
				list.add(sublist1.get_head()->value);
				sublist1.remove_head();
				i++;
			}
			else
			{
			    move_count++;
				list.add(sublist2.get_head()->value);
				sublist2.remove_head();
				j++;
			}
		}
    if(i<startl)
		list.concatenate(sublist1);
    
    if(j<endr)
		list.concatenate(sublist2);
		
	while(i<startl){
	    move_count++;
	    i++;
	}
	while(j<endr){
	    move_count++;
	    j++;
	}
}

/**
 * Clear the data.
 */
void MergeSort::clear() { data.clear(); }
