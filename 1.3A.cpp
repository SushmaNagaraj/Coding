#include <iostream>
using namespace std;

int main()
{
   cout << "a: ";
   int a;
   cin >> a;
   cout << "b: ";
   int b;
   cin >> b;
   // Print the highest power of a that is <= b.
   // For example, if a is 10 and b is 123, then you print 100
   // You may assume that a and b are positive.

   int result;
   int temp1 = 1;
   int temp2 = 1;
   
	   if(a > 0 && b > 0){
		while(temp1>0){
		temp1 = b - (a * temp2);
		if(temp1 >= 0){
		temp2 = temp2 * a;
		}
		}
		result = temp2;
	   }
   cout << endl << "Result: " << result << endl;
   return 0;
}
