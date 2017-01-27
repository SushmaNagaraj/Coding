#include <string>
#include <iostream>
using namespace std;

/**
   Mesh two strings by alternating characters from them. If one string
   runs out before the other, just pick from the longer one.
   For example, mesh("Fred", "Wilma") is "FWrieldma". 
   Use recursion. Do not use loops.
   Hint: str.substr(1) is the substring of str from position 1 to
   the end.
*/
string mesh(string a, string b);
string mesh(string a, string b)
{
if(a.length()==0 || b.length()==0) {
    return a + b;
  }
  return a.substr(0, 1) + b.substr(0, 1) + mesh(a.substr(1), b.substr(1));
}
int main(){
   string a = "fred";
   cout <<a.substr(1);
   string b = "wilma";
   string c="";
   c= mesh(a,b);
  cout << c;
}
