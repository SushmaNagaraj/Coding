#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

map<string, int> startingLetterFrequency(vector<string> words);
map<string, int> startingLetterFrequency(vector<string> words)
{
map<string, int> directory; 
vector<char> a;
vector<int> b;
int count =0;
for(int i=0;i<words.size();i++){
   a.push_back(words[i][0]);
}
for(int j=0;j<a.size();j++){
   count = 0;
   for(int k=0;k<words.size();k++){
      if(a[j]==words[k][0]){
      count++;   
      }
   }
   b.push_back(count);
}
for(int m=0;m<a.size();m++){
  stringstream ss;
  string s;
  ss << a[m];
  ss >> s;
   directory.insert(pair<string, int>(s,b[m]));
}
return directory;
}
int main()
{
   vector<string> text = { "Mary", "had", "a", "little", "lamb" };
   map<string, int> freq = startingLetterFrequency(text);
   cout << freq["M"] << endl;
   cout << "Expected: 1" << endl;
   cout << freq["l"] << endl;
   cout << "Expected: 2" << endl;
   cout << boolalpha << (freq.find("w") == freq.end()) << endl;
   cout << "Expected: true" << endl;
   vector<string> text2 = { "its", "fleece", "was", "white", "as", "snow" };
   freq = startingLetterFrequency(text2);
   cout << freq["f"] << endl;
   cout << "Expected: 1" << endl;
   cout << freq["w"] << endl;
   cout << "Expected: 2" << endl;
   cout << boolalpha << (freq.find("M") == freq.end()) << endl;
   cout << "Expected: true" << endl;
   return 0;
}
