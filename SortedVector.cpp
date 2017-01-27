#include <iostream>
#include <iterator>
#include "SortedVector.h"

using namespace std;

SortedVector::SortedVector()
{
}

SortedVector::~SortedVector()
{
}
void SortedVector:: prepend(int value){
    vector<int>::iterator i;
    i = data.begin();
    data.insert (i,value);   
}
 void SortedVector:: append(int value){
    vector<int>::iterator i;
    i = data.end();
    data.insert (i,value);    
}
bool SortedVector::find(int value) const{
   vector<int>::const_iterator i;
    for(i=data.begin();i!=data.end();++i){
    if(value == *i)
    return true;
    }
    return false;
}
int SortedVector:: get_value(int i) const{
    vector<int>::const_iterator it=data.begin();
    return it[i] ;
}
void SortedVector:: clear(){
    vector<int>::iterator it;
    for(it=data.begin();it!=data.end();){
       it=data.erase(it);
    }   
}

/***** Complete this file. *****/

bool SortedVector::check()
{
    if (data.size() == 0) return true;

    vector<int>::iterator it = data.begin();
    int prev = *it;
    while ((++it != data.end()) && (prev <= *it));
    return it == data.end();
}
