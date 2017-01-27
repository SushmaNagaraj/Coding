// Example program
#include <iostream>
#include <string>
using namespace std;

int main()
{
    int num;
    int count =0;
    
    for(num = 4;num<=100;num++){
        if(num%2 ==0){
            cout << endl;
          cout << " even numbers :" << num;  
       count++ ;
    }
}
cout << endl;
    cout << count;
}
