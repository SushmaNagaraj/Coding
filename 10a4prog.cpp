#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
using namespace std;
void digitsHelper(string str, string currentDigitGroup, vector<string>&digitGroups)
{
   if(isdigit(str[0]) && !str.empty()){
      currentDigitGroup = currentDigitGroup + str[0]; 
   }else
   {
       if(currentDigitGroup!=""){
       digitGroups.push_back(currentDigitGroup);
       currentDigitGroup = "";
       }
   }
   if(str.length()>0)
   digitsHelper(str.substr(1),currentDigitGroup,digitGroups);
}

/**
   Given a string, return a vector of all substrings consisting of 
   digits contained in it. For example, if str is "I like 7 and 13",
   return a vector containing "7" and "13".
   Use a recursive helper method. Do not use loops.
   A digit is a character between '0' and '9' inclusive.

*/
vector<string> digitGroups(string str)
{
   vector<string> result;
   digitsHelper(str, "", result);
   return result;
}
int main(){
   //string a = "2016-11-11";
   string a ="union 76";
   vector<string> c = digitGroups(a);
   for(int i=0;i<c.size();i++){
         cout << c[i] << endl;
}
}