#include<iostream>
#include<cmath>

using namespace std;

void generatePrimeNumber(int n,int primeArray[]);
void generateEvenNumber(int n,int evenArray[]);

int main(){
    int n = 100;
    int primeArray[n];
    int m =49;
    int evenArray[m];
    int tempPrimeArray[25];
    
    generatePrimeNumber(n,primeArray);
    generateEvenNumber(n,evenArray);

    cout << "Primes:" << endl << endl;

    for(int i=1;i<=100;i++){
        if(primeArray[i] == 1){
            cout << i << "   ";
        }else{
            cout << " " << "." << "   ";
        }   
        if(i% 10 == 0){
        cout << endl;
        }
    }
    cout << endl;
    int  y =0;
    for(int i=1;i<=100;i++){
        if(primeArray[i] == 1){
            tempPrimeArray[y] = i;
            y++;
        }
    }
    cout << "Test of Goldbach's Conjecture:";
   
	int i;
	int j;
	int temp=0;

	for (int k=0;k<49;k++)
	{
		i=0;
		do{
			for(j=i;tempPrimeArray[i]+tempPrimeArray[j]<=evenArray[k];j++){
				if(tempPrimeArray[i]+tempPrimeArray[j] == evenArray[k]){
					cout << endl;
				    if(evenArray[k] != temp){
						cout << endl;
						cout << " " << evenArray[k] << ":" << "  " << tempPrimeArray[i] << "  " << tempPrimeArray[j];
						temp = evenArray[k];
				}
					else
					cout << "    " << tempPrimeArray[i] << "   " << tempPrimeArray[j];		
				}else
					continue;
			}
			i++;
		}while(tempPrimeArray[i-1]+tempPrimeArray[j-1]<=evenArray[k]);
	}
    return 0;
}

void generatePrimeNumber(int n,int primeArray[]){
    for(int i =2;i<=n;i++)
    primeArray[i]=true;
    
    for(int i=2;i<=sqrt(n);i++){
        {
            for(int j=i*i;j<=n;j=j+i){
             primeArray[j]=false;   
            }
    }
    }
}

void generateEvenNumber(int m,int evenArray[]){
	int num;
	int increment = 0;
	for(num = 4;num<=100;num++){
		if(num%2 ==0){
		   evenArray[increment] = num;
		   increment++;
          }
     }
}

