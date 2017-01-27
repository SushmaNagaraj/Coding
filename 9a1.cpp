#include <iostream>
#include <algorithm>
#include <list>
#include <iterator>
using namespace std;

void removeOddPositions(list<int>& lst);


void removeOddPositions(list<int>& lst)
{
   list<int>::iterator i;
   list<int>::iterator i2;
  for(i=lst.begin() ; i!=lst.end() ; ++i)
{
    cout<<*i<<endl;
}
long int k=1;

i = lst.begin();
while ( i != lst.end() ) {
    if((k%2)==0)
    {
        i=lst.erase(i);
    }
    else
    {
        ++i;
    }
    k++;
}
}
int main()
{
   list<int> nums = { 1, 1, 2, 3, 5, 8, 13, 21, 34 };
   removeOddPositions(nums);
   copy(nums.begin(), nums.end(),
      ostream_iterator<int>(cout, " "));
   cout << endl;
   cout << "Expected: 1 2 5 13 34" << endl;
   list<int> nums2 = { 1, 2, 3, 5, 8, 13, 21, 34 };
   removeOddPositions(nums2);
   copy(nums2.begin(), nums2.end(),
      ostream_iterator<int>(cout, " "));
   cout << endl;
   cout << "Expected: 1 3 8 21" << endl;
   return 0;
}

