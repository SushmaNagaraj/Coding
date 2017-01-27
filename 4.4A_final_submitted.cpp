#include <iostream>

using namespace std;

int* reverse(int* start, int* end);


/**
   Given pointers start and end that point to the 
   first and past the last element of a segment inside
   an array, return a new array holding the reverse
   of the segment. 
*/
int* reverse(int* start, int* end)
{
		int* tempStart = start;
    	int numberOfElements = end-start;
		end = end-1;	
		int* reverseArray[numberOfElements];
		if(numberOfElements>1){
		    int i =0;
		    int j = numberOfElements-1;
		    while(start < end)
            {
                int temp = *start;
                *start = *end;
                *end = temp;
		        reverseArray[i]=start;
		        reverseArray[j]=end;
                start++;
                end--;
		        i++;
		        j--;
            } 
        }else
        reverseArray[0]= tempStart;
	return *reverseArray;
}


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
   print(reverse(a + 1, a + 5), 4);
   cout << "Expected: { -4, 9, 4, 1 }" << endl;
   
   int b[] = { 1, 4, 9, 0 };
   print(reverse(b + 1, b + 4), 3);
   cout << "Expected: { 0, 9, 4 }" << endl;

   int c[] = { 12, 13 };
   print(reverse(c, c + 1), 1);
   cout << "Expected: { 12 }" << endl;
   
   int d[] = { 14, 15 };
   print(reverse(d, d), 0);
   cout << "Expected: { }" << endl;
   
   return 0;
}


