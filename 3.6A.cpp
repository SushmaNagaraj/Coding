#include <string>
using namespace std;

/**
   Return a string that interleaves the characters in strings a and b.
   If one string is longer than the other, append the suffix.
   For example, splicing "Hello" and "Goodbye" yields "HGeololdobye".
   pilationexecution
*/
string splice(string a, string b)
{
	int alength = a.length();
    int blength = b.length();
    int size;
	string result;
    if(alength<blength){
        size = alength;
    }else{
        size = blength;
    }
    for(int i=0;i<size;i++){
		result.push_back(a[i]);
		result.push_back(b[i]);
    }
	if(alength>blength){
		for(int i = size;i<alength;i++){
			result.push_back(a[i]);
		}
    }else if(alength<blength){
		for(int i = size;i<blength;i++){
			result.push_back(b[i]);
		}   
    }
	return result;
}
-----------------------------------------


-----------------------------------------
#include <string>
#include<iostream>
using namespace std;

int main(){
    
    string a = "hello";
    string b = "universe";
	string c;
    int alength = a.length();
    int blength = b.length();
    int size;
    if(alength<blength){
        size = alength;
    }else{
        size = blength;
    }
    for(int i=0;i<size;i++){
		c.push_back(a[i]);
		c.push_back(b[i]);
    }
    if(alength>blength){
    for(int i = size;i<alength;i++){
        c.push_back(a[i]);
    }
    }else if(alength<blength){
     for(int i = size;i<blength;i++){
        c.push_back(b[i]);
    }   
    }
	cout << c;
}