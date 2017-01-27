using namespace std;

/**
 * Default constructor.
 */
 #include "RomanNumeral.h"
 #include<string>
 #include<iostream>
 #include<math.h>
RomanNumeral::RomanNumeral() : roman(""), decimal(0)
{
}

RomanNumeral::RomanNumeral(string romanString)
{
    roman = romanString;
    toDecimal();    
}

RomanNumeral::RomanNumeral(int value)
{
    decimal = value;
    toRoman();
}
int RomanNumeral::get_decimal(){
    return decimal;
}
string RomanNumeral::get_roman(){
    return roman;
}
void RomanNumeral::toRoman(){
    int tempdecimal= decimal;
    int count=3;
	while(tempdecimal>0){
		int tenspow=pow(10,count);
		int temp= tempdecimal%tenspow;
		int numval=tempdecimal-temp;
		tempdecimal=temp;
		string s="";
		switch(numval){
			case 1:
					s="I";
					break;

			case 2:
					s="II";
					break;
					
			case 3:
					s="III";
					break;

			case 4:
					s="IV";
					break;
                
			case 5:
					s="V";
					break;

			case 6:
        			s="VI";
					break;

			case 7:
					s="VII";
					break;
	
			case 8:
					s="VIII";
					break;

			case 9:
					s="IX";
					break;

			case 10:
					s="X";
					break;

			case 20:
					s="XX";
					break;      
			case 30:
					s="XXX";
					break; 
			case 40:
					s="XL";
					break; 

			case 50:
					s="L";
					break;

			case 60:
					s="LX";
					break;

			case 70:
					s="LXX";
					break;

			case 80:
					s="LXXX";
					break;
			case 90:
					s="XC";
					break;

			case 100:
					s="C";
					break;

			case 200:
					s="CC";
					break;

			case 300:
					s="CCC";
					break;
					
			case 400:
					s="CD";
					break;

			case 500:
					s="D";
					break;

			case 600:
					s="DC";
					break;

			case 700:
					s="DCC";
					break;

			case 800:
					s="DCC";
					break;

			case 900:
					s="CM";
					break;

			case 1000:
					s="M";
					break;

			case 2000:
					s="MM";
					break;
    }
    roman=roman+s;
    count--;
	}
}

void RomanNumeral::toDecimal(){

    int length = roman.length();
    int sum=0;
    int lastelement =1000;
    for(int i=0;i<length;i++){
    switch(roman[i]){  
        case 'M':
			sum += 1000;
			if(lastelement < 1000)
			{
				sum -= 2 * lastelement;
			}
			lastelement = 1000;
			break;
		case 'D':
			sum += 500;
			if(lastelement < 500)
			{
				sum -= 2 * lastelement;
			}
			lastelement = 500;
			break;
		case 'C':
			sum += 100;
			if(lastelement < 100)
			{
				sum -= 2 * lastelement;
			}
			lastelement = 100;
			break;
		case 'L':
			sum += 50;
			if(lastelement < 50)
			{
				sum -= 2 * lastelement;
			}
			lastelement = 50;
			break;
		case 'X':
			sum += 10;
			if(lastelement < 10)
			{
				sum -= 2 * lastelement;
			}
			lastelement = 10;
			break;
		case 'V':
			sum += 5;
			if(lastelement < 5)
			{
				sum -= 2 * lastelement;
			}
			lastelement = 5;
			break;
		case 'I':
			sum += 1;
			if(lastelement < 1)
			{
				sum -= 2 * lastelement;
			}
			lastelement = 1;
			break;
		}
    }
    decimal=sum;
}
RomanNumeral operator+(RomanNumeral other1,RomanNumeral other2){
      string roman1 = other1.get_roman();
      string roman2 = other2.get_roman();
      RomanNumeral a(roman1);
      RomanNumeral b(roman2);
      int decimal1 = a.decimal+ b.decimal;
      RomanNumeral total(decimal1);
      return total;
  }
RomanNumeral operator-(RomanNumeral other1,RomanNumeral other2){
      string roman1 = other1.get_roman();
      string roman2 = other2.get_roman();
      RomanNumeral a(roman1);
      RomanNumeral b(roman2);
      int decimal2 = a.decimal-b.decimal;
      RomanNumeral difference(decimal2);
      return difference;
  }
RomanNumeral operator*(RomanNumeral other1,RomanNumeral other2){
      string roman1 = other1.get_roman();
      string roman2 = other2.get_roman();
      RomanNumeral a(roman1);
      RomanNumeral b(roman2);
      int decimal3 = a.decimal*b.decimal;
      RomanNumeral mul(decimal3);
      return mul;
  }
RomanNumeral operator/(RomanNumeral other1,RomanNumeral other2){
      string roman1 = other1.get_roman();
      string roman2 = other2.get_roman();
      RomanNumeral a(roman1);
      RomanNumeral b(roman2);
      int decimal4 = a.decimal/b.decimal;
      RomanNumeral div(decimal4);
      return div;
  }
bool operator==(RomanNumeral other1,RomanNumeral other2){
   if(other1.decimal==other2.decimal)
       return true;
   else
       return false;
   }
bool operator!=(RomanNumeral other1,RomanNumeral other2){
   if(other1.decimal!=other2.decimal)
       return false;
   }
ostream& operator<<(ostream& os, RomanNumeral& dt)
{
    if(dt.roman=="")
        dt.roman=dt.get_roman();
    else if(dt.decimal==0){
        dt.toDecimal();
        dt.decimal=dt.get_decimal();
    }
    else{
        dt.decimal=dt.get_decimal();
        dt.roman=dt.get_roman();
    }
    os<<"["<<dt.decimal<<":"<<dt.roman<<"]";
    return os;
}
istream& operator>>(istream& is, RomanNumeral& dt)
{
    is>>dt.roman;
    dt.decimal=0;
    return is;
}