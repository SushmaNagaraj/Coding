

/**
   Return the nth digit from the back (that is, the factor of pow(10, n)
   in the decimal representation of the given number).
   @param num a number
   @param n the power of 10 for which we want the digit
   @return the nth digit 
   E.g. nthDigit(1729, 1) is 2 since 1729 = 9*pow(10, 0) + 2*pow(10,1) + ...
   Hint: Keep dividing by 10
*/
int nthDigit(int num, int n)
{
	int nthDigit = 0;
	for(int i=0;i<n;i++){
	num = num/10;
	}
	nthDigit = num % 10;
	return nthDigit;
}
