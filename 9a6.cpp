#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cstring>
using namespace std;

map<string, set<string>> wordsByStartingLetter(vector<string> words);
map<string, set<string>> wordsByStartingLetter(vector<string> words)
{
map<string, set<string>> directory; 
   vector<char> a;
   vector<string> b;
   string s="";
   for(int i=0;i<words.size();i++){
      a.push_back(words[i][0]);
   }
   for(int j=0;j<a.size();j++){
   s="";
   for(int k=0;k<words.size();k++){
      if(a[j]==words[k][0]){
         if(s.length()<words[k].length())
            s = s+" "+words[k];
         else if(s.length()>words[k].length())
          s = words[k]+" "+s;
      }
   }
   b.push_back(s);
}
for(int m=0;m<a.size();m++){
  stringstream ss;
  string s;
  ss << a[m];
  ss >> s;
  set<string> x;
  x.insert(b[m]);
  directory.insert(pair<string, set<string>>(s,x));
}
return directory;
}

int main()
{
   vector<string> text = { "Mary", "had", "a", "little", "lamb" };
   map<string, set<string>> startingWith = wordsByStartingLetter(text);
   set<string> as = startingWith["a"];
   copy(as.begin(), as.end(),
      ostream_iterator<string>(cout, " "));
   cout << endl;
   cout << "Expected: a" << endl;
   set<string> ls = startingWith["l"];
   copy(ls.begin(), ls.end(),
      ostream_iterator<string>(cout, " "));
   cout << endl;
   cout << "Expected: lamb little" << endl;
   cout << boolalpha << (startingWith.find("f") == startingWith.end()) << endl;
   cout << "Expected: true" << endl;
   vector<string> text2 = { "its", "fleece", "was", "white", "as", "snow" };
   startingWith = wordsByStartingLetter(text2);
   as = startingWith["a"];
   copy(as.begin(), as.end(),
      ostream_iterator<string>(cout, " "));
   cout << endl;
   cout << "Expected: as" << endl;
   set<string> ws = startingWith["w"];
   copy(ws.begin(), ws.end(),
      ostream_iterator<string>(cout, " "));
   cout << endl;
   cout << "Expected: was white" << endl;
   cout << boolalpha << (startingWith.find("M") == startingWith.end()) << endl;
   cout << "Expected: true" << endl;   
   return 0;
}
