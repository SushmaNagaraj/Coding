#include <iostream>
#include <iterator>
#include "SortedList.h"

using namespace std;

SortedList::SortedList()
{
}

SortedList::~SortedList()
{
}
void SortedList:: prepend(int value){
   list<int>::iterator i;
    i = data.begin();
    data.insert (i,value);  
}
 void SortedList:: append(int value){
    list<int>::iterator i;
    i = data.end();
    data.insert (i,value); 
}
bool SortedList::find(int value) const{
    list<int>::const_iterator i;
    for(i=data.begin();i!=data.end();++i){
    if(value == *i)
    return true;
    }
    return false;
}
int SortedList:: get_value(int i) const{
    list<int>::const_iterator it;
    list<int>::const_reverse_iterator rit;
    int sizeList = data.size();
    if(i<(sizeList/2)){
        it = data.begin();
        advance(it,i);
        return *it;
    }else{
        rit = data.rbegin();
        for(int m=sizeList;m>i+1;m--,++rit);
        return *rit;  
    }
}
void SortedList:: clear(){
    list<int>::iterator it;
    for(it=data.begin();it!=data.end();){
        it=data.erase(it);
    }
}

/***** Complete this file. *****/

bool SortedList::check()
{
    if (data.size() == 0) return true;

    list<int>::iterator it = data.begin();
    int prev = *it;
    while ((++it != data.end()) && (prev <= *it));
    return it == data.end();
}