#include <iostream>
#include <string>
#include <stdexcept>
#include <list>

using namespace std;
class List
{
public:
   virtual bool isEmpty() const = 0;
   virtual string head() const = 0;
   virtual const List& tail() const = 0;
};

class EList : public List
{
public:
   virtual bool isEmpty() const { return true; }
   virtual string head() const { throw logic_error("Empty list has no head"); }
   virtual const List& tail() const { throw logic_error("Empty list has no tail"); }
};

class NEList : public List
{
public:
   NEList(string h, const List& t) : data(h), next(t) {}   
   virtual bool isEmpty() const { return false; }
   virtual string head() const { return data; }
   virtual const List& tail() const { return next; }
private:
   string data;
   const List& next;
};
string toString(const List& lst);

inline NEList operator+=(string h, const List& t) { return NEList(h, t); }

EList nil;
/**
   Look at the recursively defined List class in list.h. 
   A List is either empty or it has a head and a tail.
   Implement the toString function that yields a comma-separated
   string of the list elements. No spaces, just commas.
   If the list is empty, return an empty string.
   If the list has length 1, return just the head.
*/
string toString(const List& lst)
{   string a;  
    if(lst.isEmpty())
    {
    return "";
    }
    if(!lst.isEmpty())
    {
       if(!lst.tail().isEmpty())
       {
          return (lst.head() + "," + toString(lst.tail()));
       }
       else
       {
          return lst.head();
       }
          
    }
}
int main()
{
   cout << toString(NEList("Mary", NEList("had", NEList("a", NEList("little", NEList("lamb", EList())))))) << endl;
   cout << "Expected: Mary,had,a,little,lamb" << endl;
   cout << toString("Its" += "fleece" += "was" += "white" += "as" += "snow" += nil) << endl;
   cout << "Expected: Its,fleece,was,white,as,snow" << endl;
   cout << toString("Fred" += "Wilma" += nil) << endl;
   cout << "Expected: Fred,Wilma" << endl;
   cout << toString("Fred" += nil);
   cout << "Expected: Fred" << endl;
   cout << toString(nil);
   cout << "Expected: Fred" << endl;
   return 0;
}
