// Example program
#include <iostream>
#include <string>
using namespace std;
#define column 5
#define row 10

int main()
{
  int array[]={10,1,90,4,67,75,70,80,3,82,89,88,2,86,93,5,63,69,73,71,82,3,100,99,98,3,89,93,91,0,5,90,85,100,78,81,4,95,91,100,97};
  int count =41;
  int score2darray[row][column];
  int i=1;
  int s=0;
  int y;
  int row1 =0;
  int counterarray[10];
  while(i<(count-1)){
      int temp = array[i];
      int temp1 = array[i];
      counterarray[s] = temp1;
      s++;
      i++;
      if(temp>0){
          int k=0;
          int arraytemp[temp]={0};
          while(temp>0){
              arraytemp[k]=array[i];
              i++;
              k++;
              temp--;
          }
           for(int m=0;m<temp1;m++){
              cout << "mmmm" << arraytemp[m] << endl;
          }
			for (y=0; y<temp1; y++)
		{   
			score2darray[row1][y]=arraytemp[y];
		 }
		}else if(temp==0)
		score2darray[row1][y] ='\0';
	    row1++;
      }
      for(int i=0;i<10;i++){
          for(int j=0;j<counterarray[i];j++){
              cout<<score2darray[i][j] << " ";
          } 
          cout << endl;
      }
      for(int g=0;g<10;g++){
       cout << counterarray[g] << endl;  
      }
  }
  