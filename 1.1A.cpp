#include <iostream>
using namespace std;

int main()
{
   cout << "Hours: ";
   int hours;
   cin >> hours;
   cout << "Minutes: ";
   int minutes;
   cin >> minutes;
   cout << "Minutes to add: ";
   int minutesToAdd;
   cin >> minutesToAdd;

   // Add minutesToAdd minutes to the given hours/minutes
   // and normalize hours/minutes to military time (so that
   // hours is between 0 and 23 and minutes between 0 and 59)

   int totalTimeinMinutes = (hours * 60)+ minutes + minutesToAdd ;
  
   while(totalTimeinMinutes<0){
   totalTimeinMinutes = totalTimeinMinutes +1440 ;
   }
   
   hours = totalTimeinMinutes/60;
   minutes = totalTimeinMinutes%60;
   hours = hours % 24;

   cout << endl << "Result: " << hours << ":" << minutes << endl;
   return 0;
}
