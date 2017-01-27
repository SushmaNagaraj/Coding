#include <string>
#include<iostream>
using namespace std;

/**
   Given a string, return a string of all digits contained in it. 
   Use recursion. Do not use loops.
   A digit is a character between '0' and '9' inclusive.
   Hint: str.substr(1) is the substring of str from position 1 to
   the end.
*/
string digits(string str)
{
   if(str.length()==0) {
    return "";
  }
  if(isdigit(str[0]))
  return str[0]+ digits(str.substr(1));
  else
  return digits(str.substr(1));
}
int main(){
   string a = "eevewv4bgb5";
   string c="";
   c= digits(a);
   cout << c;
}