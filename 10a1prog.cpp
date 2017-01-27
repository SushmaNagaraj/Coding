#include <string>
#include <iostream>
using namespace std;

/**
   Return true if the given string contains identical
   characters or is empty. Use recursion. Do not use loops.
   Hint: str.substr(1) is the substring of str from position 1 to
   the end.
*/
bool allSame(string str);

bool allSame(string str)
{
    if (str.length() == 0 || str.length() == 1)
    {
            return true;
    }
    else
    {
       if (str[0] != str[str.length() - 1])
       {
            return false;
       } else 
       {
            return allSame(str.substr(1, str.length()-1));
       }
    }
}
/*int main(){
   string str = "hhhhgg";
   bool flag = allSame(str);
   cout << boolalpha  << flag;
}*/