#include <string>
#include <vector>
#include<iostream>
#include<cmath>

using namespace std;

int altSumHelper(vector<int> v)
{
  int size = v.size();
  if(v.empty())
  return 0;
  else{
     v.erase(v.begin()+(size-1));
     return ((pow(-1,size-1)*v[size-1])+altSumHelper(v));
  }
}

/**
   Given a vector of integers, return the alternating sum 
   v[0] - v[1] + v[2] - v[3] + ...
   If the vector is empty, the alternating sum is zero.
   Use a recursive helper method. Do not use loops.

*/
int altSum(vector<int> v)
{
   return altSumHelper(v);
}
int main(){
 vector<int> numbers;
 numbers.push_back(1);
 numbers.push_back(2);
 numbers.push_back(3);
 numbers.push_back(4);
 int sum = altSum(numbers);
 cout << sum;
}