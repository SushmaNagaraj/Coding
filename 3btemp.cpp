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
    
    generatePrimeNumber(n,primeArray);
    generateEvenNumber(n,evenArray);

    cout << "Primes:" << endl;

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

    cout << "Test of Goldbach's Conjecture:" << endl;

	int i;
	int j;

	for (int k=0;i<25;i++)
	{
		i=0;
		do{
			for(j=i;primeArray[i]+primeArray[j]<=evenArray[k];j++){
				if(primeArray[i]+primeArray[j]==evenArray[k])
					cout << evenArray[k] << ":" << primeArray[i] << " " << primeArray[j];
				else
					continue;
			}
			i++;
		}while(primeArray[i-1]+primeArray[j-1]<=evenArray[k]);
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
