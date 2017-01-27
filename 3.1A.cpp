#include <iostream>
#include <iomanip>
using namespace std;
/*
  Read numbers from standard input and print the average of 
  all positive numbers, with two digits after the decimal point.
   
  The end of input is indicated by a number 0. For example, 
  if the input is 

  1 2 4 -3 5 -6 0
  
  then you print "Average: 3.00". If there are no positive elements,
  print "Average: 0.00".
*/
int input;
int sum = 0;
double average = 0.00;
int count;
int main()
{
   do{
	cin >> input;
	if(input>0){
	sum = sum + input;
	count++;
	}
   }while(input!=0);
   if(count>0 && sum>0){
   average = sum/count;
   }
   cout << "Average: ";
   cout << fixed;
   cout << setprecision(2);
   cout << average;
   return 0;
}
