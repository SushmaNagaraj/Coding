#include <iostream>
using namespace std;

int main()
{
   cout << "Hours of first time: ";
   int hours1;
   cin >> hours1;
   cout << "Minutes of first time: ";
   int minutes1;
   cin >> minutes1;
   cout << "Hours of second time: ";
   int hours2;
   cin >> hours2;
   cout << "Minutes of second time: ";
   int minutes2;
   cin >> minutes2;
   cout << endl;
   
   // If the first time comes before the second time, print BEFORE
   // If the first time comes after the second time, print AFTER
   // If they are the same time, print SAME;
     
   int totalFirstTime = (hours1 * 60 ) + minutes1;
   int totalSecondTime = (hours2 * 60 ) + minutes2;
   if(totalFirstTime<totalSecondTime)
	   cout << "BEFORE";
   else if(totalFirstTime>totalSecondTime)
	   cout << "AFTER";
   else
	   cout << "SAME";

   return 0;
}
