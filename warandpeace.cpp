#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Names to search for.
const string MAKAR = "Makar Alexeevich";
const string JOSEPH = "Joseph Bazdeev";
const string BORIS  = "Boris Drubetskoy";
const string MAKAR_FIRSTNAME = "Makar";
const string JOSEPH_FIRSTNAME = "Joseph";
const string BORIS_FIRSTNAME  = "Boris";

const string INPUT_FILE_NAME = "WarAndPeace.txt";

int main()
{
    ifstream input;
	double offset;
	string line;
	double curLine = 0;
	vector<double> lines;
	vector<double> position;
	vector<string> name;
	double templine1=0.0;
    double tempposition1=0.0;
	double templine2=0.0;
    double tempposition2 =0.0;
	bool flag1 =false;
	bool flag2 =false;
	int i;

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
	    curLine++;
		double k = line.length()-5;
		double p = line.length()-6;
		double x = line.length();
		string firstnameAtEnd1 = line.substr(k,x);
		string firstnameAtEnd2 = line.substr(p,x);
		string lastnameAtStart1 = line.substr(0,7);
		string lastnameAtStart2 = line.substr(0,10);
		
		  if(flag1){
			if(lastnameAtStart1 == "Bazdeev"){
				lines.push_back(templine1);
				position.push_back(tempposition1);
				name.push_back("Joseph Bazdeev");
			}
		}
		  if(flag2){
			if(lastnameAtStart2 == "Drubetskoy"){
			lines.push_back(templine2);
			position.push_back(tempposition2);
			name.push_back("Boris Drubetskoy");
			}
		}
		flag1 = false;
		flag2 = false;
		if ((offset = line.find(MAKAR, 0)) != string::npos) {
			lines.push_back(curLine);
			position.push_back(offset+1);
			name.push_back("Makar Alexeevich");
        }
		if(firstnameAtEnd1 == "Makar"){
			lines.push_back(curLine);
			position.push_back(line.find(MAKAR_FIRSTNAME, 0)+1);
			name.push_back("Makar Alexeevich");
		}
		if ((offset = line.find(JOSEPH, 0)) != string::npos) {
			lines.push_back(curLine);
			position.push_back(offset+1);
			name.push_back("Joseph Bazdeev");
        }
		if(firstnameAtEnd2 == "Joseph"){
			templine1 = curLine;
			tempposition1 = line.find(JOSEPH_FIRSTNAME, 0)+1;
			flag1 = true;
		}
		if ((offset = line.find(BORIS, 0)) != string::npos) {
			lines.push_back(curLine);
			position.push_back(offset+1);
			name.push_back("Boris Drubetskoy");
        }
		if(firstnameAtEnd1 == "Boris"){
			templine2 = curLine;
			tempposition2 = line.find(BORIS_FIRSTNAME, 0)+1;
			flag2 = true;
		}
    }
}
cout<< setw(5)<<"LINE"<<setw(10)<<"POSITION"<<setw(6)<<"NAME"<<endl;
for(int i=0;i<lines.size();i++){
	cout  << setw(5)<< lines[i] << setw(10) << position[i] << setw(18) << name[i] << endl;
}	
	
input.close();
}
