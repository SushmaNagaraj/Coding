#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
   cout << "Enter n: ";
   int n;
   cin >> n;
   cout << endl;
   
   // Draw the following "x in a box" pattern. There are n asterisks
   // on each side. In this example, n is 8.
   // ********
   // **    **
   // * *  * *
   // *  **  *
   // *  **  *
   // * *  * *
   // **    **
   // ********
   for(int rows = 0;rows<n;rows++){
	   for(int columns = 0;columns<n;columns++){
		   if(rows== 0 || columns == 0 || rows == n-1 || columns == n-1){
			   cout << "*";
		   }else if((rows== 1|| rows == n-2) && (columns == 1 || columns == n-2)){
			   cout << "*";
		   }else if((rows== 2|| rows== n-3) && (columns == 2 || columns == n-3)){
			   cout << "*";
		   }else if((rows== 3|| rows == n-4) && (columns == 3 || columns == n-4)){
			   cout << "*";
		   }else
		   cout << " ";
	   }
	   cout << "\n";
   }
   return 0;
}
