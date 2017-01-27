#include <vector>
#include<iostream>
using namespace std;

int main(){
    
    vector <int> a;
    int length = 
       for(int i=0;i<)
    for(int i =0;i<a.size();i++){
      cout << a[i];
    }
    vector <int> temp;
    int k1 = a.size()-3;
	int k2=  a.size()-2;
  if(a.size()>2){
	  temp.push_back(a[k2]);
	  temp.push_back(a[k1]);
	  cout << endl;
	  cout << temp [0];
	   cout << endl;
	  cout << temp[1];
	   cout << endl;
	   a.erase(a.end()-3);
	  a.erase(a.end()-2);	  
	  a.insert(a.end()-1,temp[0]);
	  a.insert(a.end()-1,temp[1]);
  }else if (a.size()==2){
       temp.push_back(a[0]);
	  temp.push_back(a[1]);
	  cout << endl;
	  cout << temp [0];
	   cout << endl;
	  cout << temp[1];
	   cout << endl;
	   a.erase(a.end()-1);
	  a.insert(a.end()-1,temp[0]);
	  a.erase(a.end()-1);	  
	  a.insert(a.end()-1,temp[1]);
  }
  for(int i =0;i<a.size();i++){
      cout << a[i];
    }
}      
     






#include <vector>
using namespace std;

/**
   Swaps the second and second-to-last value of a.
   For example, if a is {3, 1, 4, 1, 5, 9, 2, 6}
   after calling this method it is {3, 2, 4, 1, 5, 9, 1, 6}.
   If the array has length < 2, do nothing.
   @param a a vector of integers      
*/
void swap2(vector<int>& a)
{
	vector <int> temp;
    int k1 = a.size()-3;
	int k2=  a.size()-2;
	if(a.size()>2){
	temp.push_back(a[k2]);
	temp.push_back(a[k1]);
	a.erase(a.end()-3);
	a.erase(a.end()-2);	  
	a.insert(a.end()-1,temp[0]);
	a.insert(a.end()-1,temp[1]);
  }
  else if (a.size()==2){
      temp.push_back(a[0]);
	  temp.push_back(a[1]);
	  a.erase(a.end()-1);
	  a.insert(a.end()-1,temp[0]);
	  a.erase(a.end()-1);	  
	  a.insert(a.end()-1,temp[1]);
  }
}  


-------------------------------------------------------------------------------------------------------------------------------------------
sunbmitted 


#include <vector>
using namespace std;

/**
   Swaps the second and second-to-last value of a.
   For example, if a is {3, 1, 4, 1, 5, 9, 2, 6}
   after calling this method it is {3, 2, 4, 1, 5, 9, 1, 6}.
   If the array has length < 2, do nothing.
   @param a a vector of integers      
*/
void swap2(vector<int>& a)
{
   int temp;
   int secondToLast = a.size()-2;
   temp = a[1];
   a[1] = a[secondToLast];
   a[secondToLast] = temp;
}      
