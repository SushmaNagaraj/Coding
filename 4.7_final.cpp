#include <iostream>
#include <cstring>
#include<vector>
using namespace std;

char** split(char* words);

/**
   Given a '\0'-terminated character array, split it by replacing 
   each space in the character array with a '\0' and return a 
   newly allocated array of char* pointers to the resulting strings.
*/
char** split(char* words)
{
    
	char* cpyWords = words;  
    char ch = '\0';
    int count =0;
    vector<int> index;
    index.push_back(0);
    int i=1;
    while(*cpyWords) 
    {
      if(*cpyWords == ' ')
      {
         *cpyWords = ch;
         index.push_back(count+1);
         i++;
     } 
     count++;
     cpyWords++;
  }
  char* words1;
  words1= cpyWords-count; 
  char** result = new char*[index.size()];
  for(int i=0;i<index.size();i++){
	  result[i] = words1+index[i];
  }
   return result;
}

int main()
{
   char words[] = "Hello Goodbye Bonjour Arrivederci";
   char** result = split(words);
   cout << result[0] << endl;
   cout << "Expected: " << "Hello" << endl;
   cout << result[1] << endl;
   cout << "Expected: " << "Goodbye" << endl;
   cout << result[2] << endl;
   cout << "Expected: " << "Bonjour" << endl;
   cout << result[3] << endl;
   cout << "Expected: " << "Arrivederci" << endl;

   char morewords[] = "Hello  Goodbye"; // Note two spaces
   result = split(morewords);
   cout << result[0] << endl;
   cout << "Expected: " << "Hello" << endl;
   cout << strlen(result[1]) << endl;
   cout << "Expected: " << 0 << endl;
   cout << result[2] << endl;
   cout << "Expected: " << "Goodbye" << endl;
   
   return 0;
}
