#include "QuadraticProbeHT.h"
using namespace std;

QuadraticProbeHT::QuadraticProbeHT() : OpenAddressHT(), odd(1) {}
QuadraticProbeHT::QuadraticProbeHT(int size) : OpenAddressHT(size), odd(1) {}

QuadraticProbeHT::~QuadraticProbeHT() {}

/**
 * Search for a given table entry. Overrides OpenAddressHT::search().
 * @param key the key of the entry to find.
 * @param probe_count the cumulative count of probes.
 * @return the hash table index of the entry if found, else of an empty slot.
 */
int QuadraticProbeHT::search(const string& key, int& probe_count)
{
    /***** Complete this member function. *****/
    int index = hash(key);
    QuadraticProbeHT table2(size);
    while(table2.get_entry(index)!=nullptr)
    {
        if(table2.get_entry(index)->word!=key)
        {
        index = next_index(index);  // probe the next slot
        odd++;
        probe_count++;
            
        }
        
    probe_count++;
    return index;
        
    }
    probe_count++;
    return index; 
    
    
}
/**
 * Compute the index of the next hash table slot away from
 * the current slot to check. For quadratic probing, the ith probe
 * checks the slot that is i-squared away from the home slot
 * computed by hash(). Use the formula n^2 = 1 + 3 + 5 + ... + 2n-1.
 * Use private member variable odd to compute the successive squares.
 * @param index the index of the current slot.
 * @return the index of the next slot.
 */
int QuadraticProbeHT::next_index(int index)
{
    /***** Complete this member function. *****/
    int squareval=0;
    int y=0;
    for (int j=1;j<=2*odd-1;j+2)
    {
        //squareval=squareval+1;
        y=j+y;
        
    }
    return(y%size);
    
}
