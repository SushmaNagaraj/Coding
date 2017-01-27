#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include<cctype>

using namespace std;

const string INPUT_FILE_NAME = "students.txt";

/**
 * Main.
 */
int main()
{
    ifstream input;
	string words[1000]; // Vector to hold our words we read in.
	string str;
	int countNames=0;
	int countInt=0;
	string Names[1000];
	int scores[1000];
    input.open(INPUT_FILE_NAME);
    if (input.fail())
    {
        cout << "Failed to open " << INPUT_FILE_NAME << endl;
        return -1;
    }
	if(input.is_open()) {
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
	for(int i=1;i<countInt;i++){
		cout << scores[i] << endl;
	}
	int j=0;
	int z=1;
	string subject =words[0]; 
	while(z<(countNames-1)){
		Names[j] = words[z] +" "+ words[z+1];
		z=z+2;
		j++;
	}
	for(int i = 0; i < j; i++){
    cout << Names[i] << endl;
	}
}
