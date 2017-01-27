#include <iostream>
#include <cstring>

using namespace std;

char** split(char* words);

/**
   Given a '\0'-terminated character array, split it by replacing 
   each space in the character array with a '\0' and return a 
   newly allocated array of char* pointers to the resulting strings.
*/
char** split(char* words)
{

    int i = 0;
    const int count = 128;
    char* cpyWords = words;  
    char ch = '\0';
    int n =0;
    while(*cpyWords) {
      cout << *cpyWords;
      if(*cpyWords == ' '){
      *cpyWords = ch;
       n++;
     } 
     cpyWords++;
  }
    cout << n;
    char** result = new char*[count];
    char *token = strtok(words, " ");
    cout << endl;
    cout << *token;

    for( int j = 0; j < count; j++ )
    {
        result[j] = new char[40];
    }

    while(token != NULL)
    {
        strcpy(result[i], token);
        printf("[%s]\n", token);
        token = strtok(NULL, " ");
        i++;
    }
   return result;
}

int main()
{
   char morewords[] = "Hello  Goodbye"; // Note two spaces
   char** result = split(morewords);
   cout << result[0] << endl;
   cout << "Expected: " << "Hello" << endl;
   cout << strlen(result[1]) << endl;
   cout << "Expected: " << 0 << endl;
   cout << result[2] << endl;
   cout << "Expected: " << "Goodbye" << endl;
   
   return 0;
}
