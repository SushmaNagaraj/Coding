#include <vector>
using namespace std;

/**
   Rotates a vector one position to the left.
   Returns the rotated vector without modifying the
   original.
   @param a a vector of integers
   @return the rotated vector. For example, if a is {3, 1, 4, 1, 5, 9}
   then a vector {1, 4, 1, 5, 9, 3} is returned.
*/
vector<int> rot(const vector<int>& a)
{
  vector<int> temp1;
  vector<int> temp2;
  int vectorSize = a.size();
 
  for(int i=0;i<vectorSize;i++){
      temp1.push_back(a[i]);
  }
  int firstVariable = temp1[0];
  int size = temp1.size()-1;
  for(int j=0 ;j<size;j++){
      temp2.push_back(temp1[j+1]);
  }
  temp2.push_back(firstVariable);
  return temp2;
}

-------------------------------------------------------------------------------------------------------
