#include <iostream>
#include <fstream>
#include "RomanNumeral.h"

using namespace std;

const string INPUT_FILE_NAME = "RomanNumeral.txt";

void test1();
void test2(ifstream& in);

/**
 * Main. Run the two tests.
 */
int main()
{
    test1();

    ifstream in;
    in.open(INPUT_FILE_NAME);
    if (in.fail())
    {
        cout << "Input file open failed: " << INPUT_FILE_NAME;
        return -1;
    }

    test2(in);
    return 0;
}

/**
 * A test with some predefined Roman numerals
 * and an arithmetic and a relational expression.
 */
void test1()
{
    cout << "Test 1" << endl << endl;

    RomanNumeral r1(53);
    RomanNumeral r2("MCMXLIX");
    RomanNumeral r3("XXXIV");
    RomanNumeral r4(1949);

    cout << "r1 = " << r1 << endl;
    cout << "r2 = " << r2 << endl;
    cout << "r3 = " << r3 << endl;
    cout << "r4 = " << r4 << endl;

    RomanNumeral result = r1 + r2/r3;
    cout << "result = r1 + r2/r3 = "
         << r1 << " + " << r2 << " / " << r3
         << " = " << result << endl;

    cout << endl;

    if (r2 == r4)
    {
        cout << r2 << " and " << r4 << " are equal." << endl;
    }
    else
    {
        cout << r2 << " and " << r4 << " are unequal." << endl;
    }

    if (r1 == r3)
    {
        cout << r1 << " and " << r3 << " are equal." << endl;
    }
    else
    {
        cout << r1 << " and " << r3 << " are unequal." << endl;
    }
}

/**
 * A test that reads, parses, and executes
 * arithmetic expressions with Roman numerals.
 * @param in the input stream
 */
void test2(ifstream& in)
{
    cout << endl;
    cout << "Test 2" << endl << endl;

    // Read and process each expression, one per line,
    // each in the form    r1 op r2
    // where r1 and r2 are the two Roman numeral operands,
    // and arithmetic operator op is either + - * or /
    RomanNumeral r1;
    while (in >> r1)     // read r1
    {
        RomanNumeral r2, result;
        char op;
        in >> op >> r2;  // read op and r2

        switch (op)
        {
            case '+':
                result = r1 + r2;
                break;

            case '-':
                result = r1 - r2;
                break;

            case '*':
                result = r1*r2;
                break;

            case '/':
                result = r1/r2;
                break;
        }

        // Output the operands, the operator, and the result.
        cout << r1 << " " << op << " " << r2
             << " = " << result << endl;
    }
}