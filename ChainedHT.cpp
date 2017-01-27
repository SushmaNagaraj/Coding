#include <iostream>
#include <iomanip>
#include "ChainedHT.h"
using namespace std;

/**
 * Default constructor.
 */
ChainedHT::ChainedHT() : HashTable() {}

/**
 * Constructor. Initialize each table slot to nullptr.
 */
ChainedHT::ChainedHT(int size) : HashTable(size)
{
    /***** Complete this member function. *****/
    for(int i=0; i<size;i++)
    {
        table.push_back(nullptr);
    }
}

/**
 * Destructor. Delete all the entries in the collision chains.
 */
ChainedHT::~ChainedHT()
{
    /***** Complete this member function. *****/
}

/**
 * Search for a given table entry. Count the number of probes.
 * @param key the key of the entry to find.
 * @param probe_count the cumulative count of probes.
 * @return a pointer to the entry if found, else return nullptr.
 */
HashEntry *ChainedHT::search(const string& word, int& probe_count)
{
    int index = hash(word);

    /***** Complete this member function. *****/
    if(table[index]!=nullptr)
    {
        HashNode *Hnode = table[index];
        while(Hnode->entry!= nullptr)
        {
            if(Hnode->entry->word==word)
            {
                probe_count++;
                return Hnode->entry;
            }
            else if(Hnode->next==nullptr)
            {
                break;
            }
            Hnode=Hnode->next;
        }
        probe_count++;
        return nullptr;
    }
   probe_count++;
   return nullptr; 
    
}

/**
 * If an entry isn't already in the table, enter it with a
 * frequency count of 1. If the entry already exists,
 * update it by incrementing its frequency count by 1.
 * Count the number of probes from the initial search.
 * @param key the key of the entry to enter or update.
 * @return a pointer to the newly entered or updated entry.
 */
HashEntry *ChainedHT::enter(const string& word, int& probe_count)
{
    HashEntry *entry_ptr = search(word, probe_count);
    int index = hash(word);
	HashNode *tmp=table[index];

    /***** Complete this member function. *****/
        if(entry_ptr != nullptr && entry_ptr->word==word)
        {
            probe_count++;
            entry_ptr->count++;
            return entry_ptr;
        }
        else  if(table[index] == nullptr && entry_ptr == nullptr)
        {
            table[index] = new HashNode(new HashEntry(word));
            return entry_ptr;
        }
        else    
        {
            HashNode *h = table[index];
            while(h->entry != nullptr)
            {
                if(h->next==nullptr)
				    break;
                h = h->next;
            }
            table[index] = new HashNode(new HashEntry(word));
			table[index]->next=tmp;
            return tmp->entry;
	        }
        
}

/**
 * Print the contents of the hash table.
 * Skip empty table slots.
 */
void ChainedHT::dump() const
{
    /***** Complete this member function. *****/
    
    for(int j=0;j<size;j++)
    {
        HashNode *h = table[j];
        if(h!=nullptr)
        {
            cout<<j<<":"<<endl;
            while(h->entry!=nullptr)
            {
                if(h->next==nullptr)
                {
                    cout<<setw(8)<<h->entry->count<<"-"<<h->entry->word<<endl;
                    break;
                }
                else
                {
                    cout<<setw(8)<<h->entry->count<<"-"<<h->entry->word<<endl;
                    h=h->next;   
                }
            }
        }
    }
}

/**
 * Compute the average chain length.
 */
double ChainedHT::avg_chain_length() const
{
    double ccount=0;
     for (int i=0;i<get_size();i++)
 	{
 	  
 	if(table[i]!=nullptr)
     {   HashNode *Hnode = table[i];
          ccount++;
       
         while(Hnode->next!= nullptr)
         {
             ccount++;
             Hnode=Hnode->next;
         }
        
     }
   }
 return ccount/size;
//return 0;
  
}

