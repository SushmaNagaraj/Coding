#include <iostream>
#include <cstdlib>
using namespace std;
const int sentinel = -1;
int main()
{
   int result = 0;
   cout << "Enter values, -1 when done: ";
   // Read in numbers from cin. Stop when a value of -1 is entered.
   // Print the input that is closest to 100. If there are multiple
   // inputs that have the same minimal distance 100,
   // print the first one.
   // If no input was provided (other than the -1 sentinel),
   // print -1
   int input,temp1,temp2;
   cin >> input;
   if ( input == sentinel)
        result = sentinel;
   else
   {
   do
   {
	   cin >> input; 
       temp1 = abs(result - 100);
       temp2 = abs(input - 100);
       if (temp1 > temp2)
       result = input;
       else 
       result = result;
       
   } while(input != sentinel);
   }
    cout << endl << "Result: " << result << endl;
   return 0;
}
