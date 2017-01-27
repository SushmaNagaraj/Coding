#ifndef ROMANNUMERAL_H_
#define ROMANNUMERAL_H_

using namespace std;
#include<string>

class RomanNumeral
{
public:
    RomanNumeral();
    RomanNumeral(string roman);
    RomanNumeral(int value);
    int get_decimal();
    string get_roman();
    /***** Complete this class. *****/
    friend RomanNumeral operator+(RomanNumeral other1,RomanNumeral other2);
    friend RomanNumeral operator-(RomanNumeral other1,RomanNumeral other2);
    friend RomanNumeral operator*(RomanNumeral other1,RomanNumeral other2);
    friend RomanNumeral operator/(RomanNumeral other1,RomanNumeral other2);
    friend bool operator==(RomanNumeral other1,RomanNumeral other2);
    friend bool operator!=(RomanNumeral other1,RomanNumeral other2);
    friend istream& operator>>(istream& is,RomanNumeral& other);
    friend ostream& operator<<(ostream& os,RomanNumeral& other);
private:
    string roman;      // Roman numeral as a string
    int decimal;    // decimal value of the Roman numeral
    void toRoman();    // calculate string from decimal value
    void toDecimal();  // calculate decimal value from string
};

#endif /* ROMANNUMERAL_H_ */
