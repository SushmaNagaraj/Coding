// Example program
#include <iostream>
#include <string>
using namespace std;

int main()
{
    int num;
    int prime;
    int count=0;
    
    for(num = 2;num<=100;num++){
        prime =1;
        for(int j =2;j<num;j++){
            if(num%j ==0){
            prime =0;
            }
    }
    if(prime){
    cout << endl;
    cout << num;
    count++;
    }
}
cout << endl;
cout << count;
}
