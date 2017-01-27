
/**
   Given pointers start and end that point to the 
   first and past the last element of a segment inside
   an array, reverse all elements in the segment.
*/
void reverse(int* start, int* end)
{
    int * tempEnd = end-1;
    while(start < tempEnd)
    {
        int temp = *start;
        *start = *tempEnd;
        *tempEnd = temp;
        start++;
        tempEnd--;
    }  
}

#include <iostream>

using namespace std;

void reverse(int* start, int* end);

void print(int* a, int n)
{
   cout << "{";
   for (int i = 0; i < n; i++)
   {
      cout << " " << a[i];
      if (i < n - 1) cout << ",";
   }
   cout << " }" << endl;
}
   
int main()
{
   int a[] = { 5, 1, 4, 9, -4, 8, -9, 0 };
   reverse(a + 1, a + 5);
   print(a, sizeof(a) / sizeof(a[0]));
   cout << "Expected: { 5, -4, 9, 4, 1, 8, -9, 0 }" << endl;
   
   int b[] = { 1, 4, 9, 0 };
   reverse(b + 1, b + 4);
   print(b, sizeof(b) / sizeof(b[0]));
   cout << "Expected: { 1, 0, 9, 4 }" << endl;

   int c[] = { 12, 13 };
   reverse(c, c + 1);
   print(c, sizeof(c) / sizeof(c[0]));
   cout << "Expected: { 12, 13 }" << endl;
   
   int d[] = { 14, 15 };
   reverse(d, d);
   print(d, sizeof(d) / sizeof(d[0]));
   cout << "Expected: { 14, 15 }" << endl;
   
   return 0;
}


