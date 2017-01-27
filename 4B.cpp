#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

const string INPUT_FILE_NAME = "students.txt";

/**
 * Main.
 */
int main()
{
    ifstream input;
	string line;
    input.open(INPUT_FILE_NAME);
    if (input.fail())
    {
        cout << "Failed to open " << INPUT_FILE_NAME << endl;
        return -1;
    }
	if(input.is_open()) {
     while(!input.eof())
     {
		getline(input,line); 
		char * cstr = new char [line.length()+1];
		strcpy (cstr, line.c_str());
		char * p = strtok (cstr," ");
		while (p!=0)
		{
		p = strtok(NULL," ");
		//if(!isdigit(p)){
		cout << p << '\n';
		//}
		}
	 }
	}
}
