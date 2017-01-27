/*
 * Assignment #12: Compare sorting algorithms
 *
 * CMPE 180-92 Data Structures and Algorithms in C++
 * Department of Computer Engineering
 * R. Mak, Nov. 20, 2016
 */
#include <iostream>
#include <vector>
#include "ListSorter.h"

/**
 * Default constructor.
 */
ListSorter::ListSorter() : Sorter() {}

/**
 * Destructor.
 */
ListSorter::~ListSorter() {}

/**
 * Generate the data to sort. The data can be random,
 * already sorted, sorted in reverse, or all zeros.
 * @param generator the data generator.
 * @param size the number of data elements.
 */
void ListSorter::generate_data(DataGenerator *generator, int size)
{
    clear_data();

    vector<int> v;
    generator->generate_data(v, size);

    for (int i = 0; i < size; i++) data.add(v[i]);
    set_size(size);
}

/**
 * Print the data elements.
 */
void ListSorter::print_data() const
{
    Node *ptr = data.get_head();

    while (ptr != nullptr)
    {
        cout << " " << ptr->value;
        ptr = ptr->next;
    }

    cout << endl;
}

/**
 * Verify that the data is correctly sorted.
 * @return true if correctly sorted, else false.
 */
bool ListSorter::is_data_sorted() const
{
    Node *ptr = data.get_head();

    // Chase next pointers from start to end.
    while ((ptr != nullptr) && (ptr->next != nullptr))
    {
        if (ptr->value > ptr->next->value) return false;  // incorrect
        ptr = ptr->next;
    }

    return true;  // correct
}

/**
 * Clear the data.
 */
void ListSorter::clear_data()
{
    if (data.get_size() > 0) data.clear();
}