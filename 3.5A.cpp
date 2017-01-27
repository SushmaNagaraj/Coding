#include <iostream>
#include<vector>

using namespace std;

int main() {
	vector<int> a;
	int size;
	cout << "Enter the size:";
	cin >> size;
	int input;
	for(int i =0;i<size;i++){
	    cin >> input;
	    a.push_back(input);
	}
	cout << "input a :";
	for(int i =0;i<a.size();i++){
	    cout << a[i];
	}
	vector<int>row1;
	for(int i =0;i<a.size();i++){
	    row1.push_back(a[i]);
	}
	cout << endl;
	cout << "row1 :";
	for(int i =0;i<row1.size();i++){
	    cout << row1[i];
	}
	vector<vector<int>> final;
//	vector<int>row;
	for(int i=0;i<a.size();i++){
	    final.push_back(row1);
	    int temp = row1[0];
	    row1.erase(row1.begin());
	    row1.push_back(temp);	    
	}
	cout << endl;
	cout << "final:";
	for(int i=0;i<a.size();i++){
	    for(int j =0;j<a.size();j++){
	    cout << final[i][j];
	    }
	}
	
	return 0;
}

--------------------------------------------------------------------------------------------

#include <vector>
using namespace std;

/**
   Computes all rotations of a given vector.
   @param a a vector of integers
   @return a vector of all rotations, first by 0, then by
   one, two, and so on, up to n - 1, where n is the size
   of the vector. For example, if a is {1, 7, 2, 9}
   then a vector {{1, 7, 2, 9}, {7, 2, 9, 1}, {2, 9, 1, 7}, {9, 1, 7, 2}} is returned.
*/
vector<vector<int>> allrot(const vector<int>& a)
{
  vector<vector<int>>final;
  vector<int>temp;
	for(int i =0;i<a.size();i++){
	    temp.push_back(a[i]);
	}
	for(int i=0;i<a.size();i++){
	    final.push_back(temp);
	    int tempFirstVariable = temp[0];
	    temp.erase(temp.begin());
	    temp.push_back(tempFirstVariable);	    
	} 
	return final;
}

