#include <iostream>
#include <cstdlib>
using namespace std;
const int sentinel = -1;
const int size = 5;

int main()
{
   //int result = 0;
   int number[size];
   int temp1[size];
   cout << "Enter values, -1 when done: ";
   // Read in numbers from cin. Stop when a value of -1 is entered.
   // Print the input that is closest to 100. If there are multiple
   // inputs that have the same minimal distance 100,
   // print the first one.
   // If no input was provided (other than the -1 sentinel),
   // print -1
	for(int i=0;i<size;i++){
		cin >> number[i];
		if(number[i]!= sentinel)
		temp1[i] = abs(number[i]-100);
		if (number[i] == sentinel)
		break;
      
	}
	for(int i = 0; i < size ;i++){
	    cout << temp1[i];
	}
	int smallest = temp1[0];
	for(int j =1;j<size;j++){
	    cout << + " j value" << j;
	    if (temp1[j] <= smallest) {
        smallest = temp1[j];
		cout << smallest ;
    } 
	}
	cout << endl << "Result: " << smallest << endl;
	return 0;
}