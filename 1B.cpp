#include <iostream>
#include <fstream>
#include <string>


using namespace std;

const string INPUT_FILE_NAME = "GettysburgAddress.txt";

int main()
{
    char character ='l';
    string line;
    int line_count = 0;
    int character_count = 0;
    int word_count = 0;
    int lower_count = 0;
    int upper_count = 0;
    int space_count = 0;
    int punctuation_count = 0;

    ifstream input;
    input.open(INPUT_FILE_NAME);
    if (input.fail())
    {
        cout << "Failed to open " << INPUT_FILE_NAME << endl;
        return -1;
    }

    cout << "Statistics for file " << INPUT_FILE_NAME << ":" << endl;
    cout << endl;

     while(!input.eof())
     {
	getline(input,line);
	line_count++;
	char prev_character = '\0';
	for(char& character : line) {
		character_count++;
		if (prev_character =='\0'){
			word_count++;
		}	
		if((prev_character=='-')&&(character =='-'))
			word_count++;
		if(character ==' '){
			space_count++;
			word_count++;
		}
		if(character =='\n')
			word_count++;
		if((character == ',') || (character =='-')||(character =='.'))
			punctuation_count++;
		if((character>=65)&&(character<=90)){
			upper_count++;
		}
		if((character>=97)&&(character<=122)){
			lower_count++;
		}
	prev_character = character;
	}
}

    cout << "Lines: " << line_count-1<< endl;
    cout << "Characters: " << character_count << endl;
    cout << "Words: " << word_count << endl;
    cout << "Lower-case letters: " << lower_count << endl;
    cout << "Upper-case letters: " << upper_count << endl;
    cout << "Spaces: " << space_count << endl;
    cout << "Punctuation marks: " << punctuation_count << endl;
}