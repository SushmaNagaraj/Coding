#include <string.h>

/**
   Compuute a string that interleaves the characters in strings s and t.
   If one string is longer than the other, append the suffix.
   For example, splicing "Hello" and "Goodbye" yields "HGeololdobye".
   Place the result into the character array r of size rlen.
   If the result does not fit, truncate it to rlen - 1 characters 
   and a '\0' terminator.
*/
void splice(const char s[], const char t[], char r[], int rlen)
{
	int slength = strlen(s);
    int tlength = strlen(t);
    int size;
	int tempArraySize = slength + tlength; 
	char tempArray[tempArraySize]="";
    if(slength<tlength){
        size = slength;
    }else{
        size = tlength;
    }
    int j = 0 ;
    int tempSize = size *2;
    for(int i=0;i<size;i++){
        if(j < tempSize){
			tempArray[j] = s[i];
  			j = j+1;
			tempArray[j] = t[i];
			j = j+1;
        }
    }
    if(slength>tlength){
		for(int i = size;i<slength;i++){
			tempArray[tempSize] = s[i];
			tempSize = tempSize+1;
		}
    }else if(slength<tlength){
		for(int i = size;i<tlength;i++){
			tempArray[tempSize] = t[i];
			tempSize = tempSize+1;
		}   
    }   
    for(int i =0;i<rlen-1;i++){
		if(i<tempArraySize){
			r[i]=tempArray[i];
		}
    }
    r[tempArraySize]=0;
}
-------------------------------------------------------------------

-----------------------------------------
#include <string>
#include<iostream>
using namespace std;

int main(){
    
    string a = "hello";
    string b = "Goodbye";
	string c;
    int alength = a.length();
    int blength = b.length();
    int size;
    if(alength>blength){
        size = alength;
    }else{
        size = blength;
    }
    for(int i=0;i<size;i++){
		r.push_back(a[i]);
		r.push_back(b[i]);
    }
	cout << c;
}




#include <string.h>
#include<iostream>
using namespace std;

int main(){
    
    const char s[] = "goodbye";
    const char t[] = "cruel world"; 
    char r[15];
    int rlen = 15;
    int slength = strlen(s);
    int tlength = strlen(t);
    int size;
	char tempArray[15];
    if(slength<tlength){
        size = slength;
    }else{
        size = tlength;
    }
    int j = 0 ;
    int temp = size *2;
    for(int i=0;i<size;i++){
        if(j < temp){
        tempArray[j] = s[i];
        cout << endl;
        cout << "jvalue--->" << j << "value--->" << tempArray[j] ;
        cout << endl;
  	j = j+1;
	tempArray[j] = t[i];
	cout << endl;
	cout << "jvalue--->" << j << "value--->" << tempArray[j] ;
	j = j+1;
        }
    }
    int k = strlen(tempArray); 
    cout << "k value" << k;
    if(slength>tlength){
	for(int i = size;i<slength;i++){
		tempArray[k] = s[i];
		k = k+1;
	}
	for(int i =0;i<strlen(tempArray);i++)
	cout << tempArray[i];
    }else if(slength<tlength){
	for(int i = size;i<tlength;i++){
		tempArray[k] = t[i];
		k = k+1;
	}
    }
    int m = strlen(tempArray); 
    cout << " m value " << m;
    int i ;
    for(i=0;i<rlen-1;i++){
		if(i < m){
		    cout << endl;
		//   cout << "jvalue--->" << i << "value--->" << tempArray[i] ;
			cout << tempArray[i];
			
		}
    }
}
