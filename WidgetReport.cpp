#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class WidgetReport
{
    /***** Complete this class. *****/
    
public:
    WidgetReport(ifstream& in):input(in){};  // constructor
    void print(ifstream& input);

private:
    ifstream& input;  // reference to the input stream
    string line;
    string  names [1000];
    string empidnames[1000];
	int  integers[1000];
	string headerNames[1000];
	int state[50];
	int plant[50];
	int dept[50];
	int empid[50];
	int count[50];
    int countNames = 0;
    int countInt =0;
    int headerNamesCount = 0;
	int rowcount =0;
	int empidcount =0;
	int totalstate[2];
	int s=0;
	int totalplant[4];
	int p=0;
	int totaldept[7];
	int d=0;
	int total =0;
};

const string INPUT_FILE_NAME = "widgets.txt";

int main()
{
	ifstream input;
    WidgetReport wr(input);
	wr.print(input);
	return 0;
}

void WidgetReport::print(ifstream& input)
{
	input.open("widgets.txt");
    if (input.fail())
    {
        cout << "Failed to open " << INPUT_FILE_NAME << endl;
        exit(0);
    }
    if(input.is_open()) {
        getline(input,line);
		istringstream iss(line);
		while (iss){
        string words;
        iss >> words;
        headerNames[headerNamesCount]=words;
        headerNamesCount++;
	}
	
    while(!input.eof())
    {
		string words;
		getline(input,line);
		istringstream iss(line);
		while (iss){
			string words;
			iss >> words;
			if((!(isdigit(words[0])) && (words!=" " && words!="\0" ))){
				names[countNames]=words;
				countNames++;
			}
			if((isdigit(words[0])) && (words!=" " && words!="\0")){
				int value = atoi(words.c_str());
				integers[countInt]=value;
				countInt++;
			}
		}
	  }

     for(int i=0;i<countNames-1;i=i+2){
     	empidnames[empidcount]=names[i]+" "+names[i+1];
     	empidcount++;
     }
     for(int i=0;i<countInt-1;i++){
		state[rowcount]=integers[i];
		i++;
		plant[rowcount]=integers[i];
		i++;
		dept[rowcount]=integers[i];
		i++;
		empid[rowcount]=integers[i];
		i++;
		count[rowcount]=integers[i];
		rowcount ++;
	 }
     for(int i=0;i<headerNamesCount-3;i++){
		cout << headerNames[i] << " ";
     }
     	cout << headerNames[5] << "  ";
     	cout << headerNames[4];
		cout <<endl<<endl;

	totalstate[0]=count[0];
	for(int i=0;i<rowcount;i++){
		if(state[i]==state[i+1]){
				totalstate[s]=totalstate[s]+count[i+1]; 
		}else{
				s++;
				totalstate[s]=count[i+1];
		}
	}
	totalplant[0]=count[0];
	for(int i=0;i<rowcount;i++){
		if(plant[i]==plant[i+1]){
				totalplant[p]=totalplant[p]+count[i+1]; 
		}else{
				p++;
				totalplant[p]=count[i+1];
		}
	}

	totaldept[0]=count[0];
	for(int i=0;i<rowcount;i++){
		if((plant[i]==plant[i+1])&&(dept[i]==dept[i+1])){
				totaldept[d]=totaldept[d]+count[i+1]; 
		}else{
				d++;
				totaldept[d]=count[i+1];
		}
	}
	
	for(int i=0;i<rowcount;i++)
		total =total+count[i];
			
	for(int i=0;i<2;i++)
		cout << "   "<<state[i] <<"    "<<plant[i]<<"   "<<dept[i]<<"   "<<empid[i]<<"     "<<count[i]<<"  "<<empidnames[i]<< endl; 
		cout<<endl;
		cout << "                          "<<totaldept[0]<<"  "<<"TOTAL FOR DEPT  "<<dept[0]<<" "<<"*"<<endl;
		cout<<endl;
		
	for(int i=2;i<=2;i++)
		cout << "   "<<state[i] <<"    "<<plant[i]<<"   "<<dept[i]<<"   "<<empid[i]<<"     "<<count[i]<<"  "<<empidnames[i]<< endl;
		cout<<endl;
		cout << "                           "<<totaldept[1]<<" "<<"TOTAL FOR DEPT  "<<dept[2]<<" "<<"*"<<endl;
		cout << "                          "<<totalplant[0]<<" "<<"TOTAL FOR PLANT "<<plant[2]<<" "<<"**"<<endl;
		cout<<endl;
		
	for(int i=3;i<5;i++)
		cout << "   "<<state[i] <<"    "<<plant[i]<<"   "<<dept[i]<<"   "<<empid[i]<<"     "<<count[i]<<"  "<<empidnames[i]<< endl;
		cout<<endl;
		cout << "                          "<<totaldept[2]<<" "<<"TOTAL FOR DEPT  "<<dept[3]<<" "<<"*"<<endl;
		cout<<endl;
		
	for(int i=5;i<7;i++)
		cout << "   "<< state[i] <<"    "<<plant[i]<<"   "<<dept[i]<<"   "<<empid[i]<<"    "<<count[i]<<"  "<<empidnames[i]<< endl;
		cout<<endl;
		cout << "                          "<<totaldept[3]<<" "<<"TOTAL FOR DEPT  "<<dept[5]<<" "<<"*"<<endl;
		cout << "                          "<<totalplant[1]<<" "<<"TOTAL FOR PLANT "<<plant[5]<<" "<<"**"<<endl;
		cout << "                          "<<totalstate[0]<<" "<<"TOTAL FOR STATE "<<state[5]<<" "<<"***"<<endl;
		cout<<endl;
		
	for(int i=7;i<10;i++)
		cout <<"   "<< state[i] <<"    "<<plant[i]<<"   "<<dept[i]<<"   "<<empid[i]<<"    "<<count[i]<<"  "<<empidnames[i]<< endl;
		cout<<endl;
		cout << "                          "<<totaldept[4]<<" "<<"TOTAL FOR DEPT  "<<dept[7]<<" "<<"*"<<endl;
		cout<<endl;
		
	for(int i=10;i<=10;i++)
		cout << "   "<<state[i] <<"    "<<plant[i]<<"   "<<dept[i]<<"   "<<empid[i]<<"     "<<count[i]<<"   "<<empidnames[i]<< endl;
		cout<<endl;
		cout << "                           "<<totaldept[5]<<" "<<"TOTAL FOR DEPT  "<<dept[10]<<" "<<"*"<<endl;
		cout << "                          "<<totalplant[2]<<" "<<"TOTAL FOR PLANT "<<plant[10]<<" "<<"**"<<endl;
		cout<<endl;
		
	for(int i=11;i<=11;i++)
		cout << "   "<<state[i] <<"    "<<plant[i]<<"   "<<dept[i]<<"   "<<empid[i]<<"     "<<count[i]<<"   "<<empidnames[i]<< endl;
		cout<<endl;
		cout << "                           "<<totaldept[6]<<" "<<"TOTAL FOR DEPT  "<<dept[11]<<" "<<"*"<<endl;
		cout << "                           "<<totalplant[3]<<" "<<"TOTAL FOR PLANT "<<plant[11]<<" "<<"**"<<endl;
		cout << "                          "<<totalstate[1]<<" "<<"TOTAL FOR STATE "<<state[11]<<" "<<"***"<<endl;
		cout<<endl;
		cout << "                          "<<total<<" "<<"GRAND TOTAL    "<<"    "<<"****"<<endl;			
     }

    input.close();
}

/***** Put member function definitions after the main. *****/
