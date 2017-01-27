#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <stdio.h>
#include <string.h>
#include <sstream>

#include "LinearProbeHT.h"
#include "QuadraticProbeHT.h"
#include "ChainedHT.h"
using namespace std;

void process_input(ifstream& in, int& line_count, int& word_count,
                   map<string, int>& stl_map,
                   LinearProbeHT& linear_table, int& linear_probe_count,
                   QuadraticProbeHT& quadratic_table, int& quadratic_probe_count,
                   ChainedHT& chained_table, int& chained_probe_count);

void enter_map(map<string, int>& stl_map, const string& word);
void dump_map(map<string, int>& stl_map);

void print_stats(int word_count, int line_count, map<string, int>& stl_map,
                LinearProbeHT& linear_table, int linear_probe_count,
                QuadraticProbeHT& quadratic_table, int quadratic_probe_count,
                ChainedHT& chained_table, int chained_probe_count);

void test_word(const string& word, map<string, int>& stl_map,
               LinearProbeHT& linear_table, QuadraticProbeHT& quadratic_table,
               ChainedHT& chained_table);
void print_entry(HashEntry *entry_ptr, int probe_count);

void dump_data(map<string, int>& stl_map,
               LinearProbeHT& linear_table,
               QuadraticProbeHT& quadratic_table,
               ChainedHT& chained_table);

const string INPUT_FILE_NAME = "USConstitution.txt";

const string WORDS_TO_TEST[] = {"amendment", "article", "ballot", "citizens",
                                "congress", "constitution", "democracy",
                                "electors", "government", "law", "legislature",
                                "people", "president", "representatives",
                                "right", "trust", "united", "states", "vice",
                                "vote"};

/**
 * Main.
 * @param argc the number of command-line arguments.
 * @param argv the array of pointers to the argument strings.
 */
int main(int argc, char *argv[])
{
    int line_count = -1;
    int word_count = 0;

    int linear_probe_count = 0;
    int quadratic_probe_count = 0;
    int chained_probe_count = 0;

    bool dump_flag = true;
    if (argc > 1) dump_flag = strcmp(argv[1], "--dump") == 0;

    ifstream in;
    in.open(INPUT_FILE_NAME);
    if (in.fail())
    {
        cout << "Failed to open " << INPUT_FILE_NAME << endl;
        return -1;
    }

    // Map and hash tables.
    map<string, int> stl_map;
    LinearProbeHT    linear_table(1150);
    QuadraticProbeHT quadratic_table(2281);
    ChainedHT        chained_table(500);

    process_input(in, line_count, word_count, stl_map,
                  linear_table, linear_probe_count,
                  quadratic_table, quadratic_probe_count,
                  chained_table, chained_probe_count);
    in.close();

    print_stats(word_count, line_count, stl_map,
              linear_table, linear_probe_count,
                quadratic_table, quadratic_probe_count,
                chained_table, chained_probe_count);

    for (string word : WORDS_TO_TEST)
    {
        test_word(word, stl_map, linear_table, quadratic_table,
                 chained_table);
    }

    if (dump_flag)
    {
        dump_data(stl_map, linear_table, quadratic_table, chained_table);
    }

    cout << endl << "Done!" << endl;
    return 0;
}

/**
 * Read the input file and enter its words into the map and hash tables.
 * Each word is first converted to all lower case letters.
 * @param in the input stream.
 * @param stl_map an STL map data structure.
 * @param linear_table the hash table using linear probing.
 * @param quadratic_table the hash table using quadratic probing.
 * @param chained_table the hash table using collision chains.
 */
void process_input(ifstream& in, int& line_count, int& word_count,
                   map<string, int>& stl_map,
                   LinearProbeHT& linear_table, int& linear_probe_count,
                   QuadraticProbeHT& quadratic_table, int& quadratic_probe_count,
                   ChainedHT& chained_table, int& chained_probe_count)
{
   
	char character ='l';
    string line;
    string words;
    /* string words1;
    string words2; */
    
    
  
    ifstream input;
    input.open(INPUT_FILE_NAME);
    // if (input.fail())
    // {
    //     cout << "Failed to open " << INPUT_FILE_NAME << endl;
    //     return -1;
    // }

   // cout << "Statistics for file " << INPUT_FILE_NAME << ":" << endl;
    //cout << endl;

     while(!input.eof())
     {
	  getline(input,line);
	  line_count++;
     } 
    // cout << line_count;
     string word;
     int i;
     char c;
    /*  ifstream inputnew;
     inputnew.open(INPUT_FILE_NAME); */
     while(in >> words)
     {
         string word = "";
         int flag =0;
         for( i=0;i<words.size();i++)
         {
             
                 
                 if(ispunct(words[i]))
                 {
                     if(words[i] == '-')
                         {
                             word = "";
                             for(int k=0;k<i;k++)
                             {
                                 c = tolower(words[k]);
                                 
                                 word = word+c;
                                 
                             }
                         word_count++;
                    enter_map(stl_map, word);
    				linear_table.enter(word, linear_probe_count);
    				quadratic_table.enter(word, quadratic_probe_count);
    				chained_table.enter(word, chained_probe_count);
                         word = "";
                         for(int k=i+1;k<words.size();k++)
                         {
                             c = tolower(words[k]);
                                 
                                 word = word+c;
                         }
                       // word_count++;
                    enter_map(stl_map, word);
    				linear_table.enter(word, linear_probe_count);
    				quadratic_table.enter(word, quadratic_probe_count);
    				chained_table.enter(word, chained_probe_count);
                         flag = 2;
                         break;
                         }
                         if(words[i] == '(')
                         {
                              word = "";
                             for(int k=i+1;k<words.size();k++)
                             {
                               
                                 c = tolower(words[k]);
                                 
                                 word = word+c;
                                 flag =1;
                                
                             }
                         }
                     else{
                     words[i] == '\0';
                     flag =1;
                     break;
                     }
                 }
             
             if(isalpha(words[i])&&!(ispunct(words[i])))
             {
                 c = tolower(words[i]);
                         
                         word = word+c;
                flag=1;
             }
             
            
         
            if(isdigit(words[i]) && isalpha(words[i+1]))
            {
                c = tolower(words[i+1]);
                         
                         word = word+c;
                //cout << words<<endl;
                // word_count++;
                flag=1;
                break;
            }
             
            
         
         }
     
         if(flag==1){
         enter_map(stl_map, word);
    				linear_table.enter(word, linear_probe_count);
    				quadratic_table.enter(word, quadratic_probe_count);
    				chained_table.enter(word, chained_probe_count);
          word_count++;
         }
         
          
     
     
}   
}

/**
 * Enter a word into the STL map.
 * If the word is already in the map, update its count.
 * @param stl_map the map.
 * @param word the word.
 */
void enter_map(map<string, int>& stl_map, const string& word)
{
    map<string, int>::iterator it;
    it=stl_map.begin();
    int flag=0;
    int count=1;
    while(it!=stl_map.end())
   //for (it=stl_map.begin();it!=stl_map.end();++it)
   {
       if(it->first==word)
	   {
	   	 flag=1;
	   	 //count++;
	      //stl_map.insert(pair<string, int>(word, count));
	      it->second++;
	       break;
	   }
   
	   //else 
	  // {
	     //  stl_map.insert(pair<string, int>(word, count));
	   // }
	    it++;
   }
   if(flag==0)
   {
   	stl_map.insert(pair<string, int>(word, count));
   }
   
   
}

/**
 * Dump the contents of the STL map.
 * @param stl_map the map.
 */
void dump_map(map<string, int>& stl_map)
{
    map<string, int>::iterator it;
  for(it=stl_map.begin();it!=stl_map.end();++it)
  {
      cout <<it->second<<"-"<<it->first << endl;
  }
}

/**
 * Print statistics.
 * @param word_count the total count of words in the input file.
 * @param line_count the number of lines in the input file.
 * @param stl_map the map.
 * @param linear_table the hash table using linear probing.
 * @param linear_probe_count the total number of probes in the linear table.
 * @param quadratic_table the hash table using quadratic probing.
 * @param quadratic_probe_count the total number of probes in the quadratic table.
 * @param chained_table the hash table using collision chains.
 * @param chained_probe_count the total number of probes in the chained table.
 */
void print_stats(int word_count, int line_count, map<string, int>& stl_map,
                  LinearProbeHT& linear_table, int linear_probe_count,
                  QuadraticProbeHT& quadratic_table, int quadratic_probe_count,
                  ChainedHT& chained_table, int chained_probe_count)
 {
     /***** Complete this function. *****/

     double dbl_word_count = word_count;
     double linear_probe_avg = (double (linear_probe_count)/word_count) ;
     double quadratic_probe_avg =(double (quadratic_probe_count)/word_count) ;

     double chained_length_avg  = chained_table.avg_chain_length();
     double chained_probe_avg = (double (chained_probe_count)/word_count);

     cout << "Statistics for file " << INPUT_FILE_NAME << ":" << endl;
     cout << endl;

     cout << "                   Lines: " << line_count << endl;
     cout << "             Total words: " << word_count << endl;
     cout << "          Distinct words: " << stl_map.size()<<endl;
     cout << endl;
     cout << "       Linear table size: " << linear_table.get_size() << endl;
     cout << "     Total linear probes: " << linear_probe_count << endl;
     cout << "   Average linear probes: " << linear_probe_avg << endl;
     cout << endl;
     cout << "    Quadratic table size: " << quadratic_table.get_size() << endl;
     cout << "  Total quadratic probes: " << quadratic_probe_count << endl;
     cout << "Average quadratic probes: " << quadratic_probe_avg << endl;
     cout << endl;
     cout << "      Chained table size: " << chained_table.get_size() << endl;
     cout << "    Average chain length: " << chained_length_avg << endl;
     cout << "    Total chained probes: " << chained_probe_count << endl;
     cout << "  Average chained probes: " << chained_probe_avg << endl;
 }

/**
 * Test a word by searching the STL map and the hash tables.
 * For the STL map and the hash tables, print each found word
 * in the form count-word. For the hash tables, also print the
 * count of probes to find the word.
 * @param word the word to search for.
 * @param stl_map an STL map data structure.
 * @param linear_table the hash table using linear probing.
 * @param quadratic_table the hash table using quadratic probing.
 * @param chained_table the hash table using collision chains.
 */
void test_word(const string& word, map<string, int>& stl_map,
               LinearProbeHT& linear_table, QuadraticProbeHT& quadratic_table,
               ChainedHT& chained_table)
{
    cout << endl << "Tests with the word '" << word << "':" << endl << endl;
    int linearP=0;
	int QuadraticP=0;
	int ChainedP=0;
	int Lindexsearch= linear_table.search(word, linearP);	
	int Qindexsearch= quadratic_table.search(word, QuadraticP);
	HashEntry *Cindexsearch = chained_table.search(word, ChainedP);
    

if(stl_map[word]==0)
	{	    
		cout<<"                   STL map:" << "** Not found. **" << endl;
		cout<<"   Linear probe hashtable:" << "** Not found. **" << endl;
		cout<<"Quadratic probe hashtable:" << "** Not found. **" << endl;
		cout<<"        Chained hashtable:" << "** Not found. **" << endl;    
	}
    

    else
	{
	cout<< "                 STL Map: "<<stl_map[word]<<"-"<<word<<endl;
	cout << "   Linear probe hashtable: "; print_entry(linear_table.get_entry(Lindexsearch), linearP);
	cout << "Quadratic probe hashtable: "; print_entry(quadratic_table.get_entry(Qindexsearch), QuadraticP);
	cout << "       Chained hashtable : "; print_entry(Cindexsearch, ChainedP);
	}
}

/**
 * Print a hash table entry if it exists.
 * Print each entry in the form count-word (n probes)
 * @param entry_ptr the pointer to the entry.
 * @param probe_count the probe count to find the entry.
 */
void print_entry(HashEntry *entry_ptr, int probe_count)
{
if (entry_ptr!=nullptr)
	cout <<  entry_ptr->count << "-" << entry_ptr->word << " " << "(" << probe_count << " probes)" << endl;
}

/**
 * Dump the data in the STL map and in the hash tables.
 * Print each entry in the form count-word
 * For the hash tables, also print the table slot index,
 * and skip empty table slots.
 * @param linear_table the hash table using linear probing.
 * @param quadratic_table the hash table using quadratic probing.
 * @param chained_table the hash table using collision chains.
 */
void dump_data(map<string, int>& stl_map,
               LinearProbeHT& linear_table,
               QuadraticProbeHT& quadratic_table,
               ChainedHT& chained_table)
{
    cout << endl << "Map dump:" << endl << endl;
    dump_map(stl_map);

    cout << endl << "Linear probe hash table dump:" << endl << endl;
    linear_table.dump();

    cout << endl << "Quadratic probe hash table dump:" << endl << endl;
    quadratic_table.dump();

    cout << endl << "Chained hash table dump:" << endl << endl;
    chained_table.dump();
}
