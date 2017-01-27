#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include<cctype>

using namespace std;
#define column 5
#define row 10

void getdata(ifstream& input);
void print(string *Names,int columncountarray[],int **score2darray,string subject);
typedef int* Intptr;
const string INPUT_FILE_NAME = "students.txt";

/**
 * Main.
 */
int main()
{
	
    ifstream input;
    getdata(input);
}
void getdata(ifstream& input)
{
	string words[1000]; // Vector to hold our words we read in.
	string str;
	int countNames=0;
	int countInt=0;
	//string Names[1000];
	int scores[1000];
    input.open("students.txt");
    if (input.fail())
    {
        cout << "Failed to open " << INPUT_FILE_NAME << endl;
    }
	if(input.is_open()) 
	{
     while(!input.eof())
     {
		 while (input >> str) // Will read up to eof() and stop at every
		{        // whitespace it hits. (like spaces!)
		if(!isdigit(str[0])){
		words[countNames]=str;
		countNames++;
		}
		if(isdigit(str[0])){
		int value = atoi(str.c_str());
		scores[countInt]=value;
		countInt++;
		}
		}
	 }
	}
	int columncountarray[countNames];
	string subject =words[0]; 
	int rows= scores[0];
	int j=0;
	int z=1;
	Intptr *score2darray = new Intptr[countNames];
	string * Names = new string[countNames];
	while(z<(countNames-1))
	{
		Names[j] = words[z] +" "+ words[z+1];
		z=z+2;
		j++;
	}

	int i=1;
	int y;
	int row1 =0;
	int s=0;
	while(i<(countInt-1)){
	int temp = scores[i];
	int temp1 = scores[i]; 
	columncountarray[s] = temp1;
	score2darray[row1] = new int[temp1];
    s++;
	i++;
	if(temp>0){
          int k=0;
          int arraytemp[temp];
          while(temp>0){
              arraytemp[k]=scores[i];
              i++;
              k++;
              temp--;
          }
	for (y=0; y<temp1; y++)
	{   
	score2darray[row1][y]=arraytemp[y];
	}
	}
	else if(temp==0)
	score2darray[row1][y]='\0';
	row1++;
      }
      
    print(Names,columncountarray,score2darray,subject);
    
    for(i=0;i<countNames;i++)
    	delete [] score2darray[i];
    	delete [] score2darray;
    	
    	delete [] Names;
}
void print(string *Names,int columncountarray[],int **score2darray,string subject)
{
	  cout << "STUDENT SCORES FOR " + subject << endl<<endl;
	  for(int i=0;i<10;i++)
	  {
		  cout <<Names[i] << endl;
		   if(columncountarray[i]==0){
				  cout << "(none)";
			  }
          for(int j=0;j<columncountarray[i];j++){
              cout<<"   "<< score2darray[i][j] << " ";
          } 
          cout << endl<<endl;
      }
}
