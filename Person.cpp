#include <iostream>
#include <string>
#include<iomanip>
#include <vector>
#include "Person.h"
using namespace std;

/***** Write the constructors. *****/
    Person::Person(){}
    
    Person::Person(int level, string name){
        this->level = level;
        this->name = name;
    }
     Person::Person(int level, string name, string spouse){
        this->level = level;
        this->name = name;
        this->spouse_name = spouse;
    }

/**
 * Destructor
 */
Person::~Person()
{
/*    /***** Complete this destructor: Recursively delete all the nodes. *****/

      for(int i = 0; i < children.size();++i)
      {
        Person *pointer = children[i];
        pointer ->~Person();
      }
}

/**
 * Have a child.
 * @param child pointer to the new child.
 */
void Person::have_child(Person *child)
{
    /***** Complete this member function. *****/
    this->children.push_back(child);
}

/**
 * Print a person.
 */
void Person::print() const
{
    if(spouse_name!="")
        cout <<name <<" "<<"("<<spouse_name<<")"<<endl;
    else
        cout <<name <<endl;
    for(int i=0;i<children.size();i++)
    {
        print_bar();
        children[i]->print();
    }
}

/**
 * Print the vertical bar.
 */
void Person::print_bar() const
{
    if(this->level==0){
          cout <<"|"<<"\n"<<"+---";
    }else if(this->level==1){
        if(this->name == "Tom")
        cout <<"    |"<<"\n"<<"    +---";
        else
        cout <<"|"<<"   |"<<"\n"<<"|"<<"   +---";
    }else if(this->level==2){
        if(this->name == "Charlotte")
        cout <<"        "<< "|"<<"\n"<<"        "<<"+---";
        else
        cout <<"    |"<<"   "<< "|"<<"\n"<<"    "<<"|"<<"   "<<"+---";
    }
}
