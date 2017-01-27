#include <iostream>
#include <algorithm>
#include <list>
#include <iterator>
using namespace std;

void crossOver(list<int>& a, int n, list<int>& b, int m);
void crossOver(list<int>& a, int n, list<int>& b, int m)
{
  list<int>::iterator item1=a.begin();
  list<int>::iterator item2 =b.begin();
  advance(item1, n);
  advance(item2, m);
  for(;(item1!=a.end()&&item2!=b.end());){
   swap(*item1, *item2);
   advance(item1, 1);
   advance(item2, 1);
}
}
int main()
{
   list<int> nums1 = { 1, 2, 3, 4 };
   list<int> nums2 = { 5, 6, 7 };
   crossOver(nums1, 2, nums2, 1);
   copy(nums1.begin(), nums1.end(),
      ostream_iterator<int>(cout, " "));
   cout << endl;
   cout << "Expected: 1 2 6 7" << endl;
   copy(nums2.begin(), nums2.end(),
      ostream_iterator<int>(cout, " "));
   cout << endl;
   cout << "Expected: 5 3 4" << endl;
   return 0;
}
