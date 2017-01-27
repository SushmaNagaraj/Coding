/**
   We have four t-shirts, one of which is extra-large. 
   Write a function that takes the weights of four people
   and returns the weight of the person who should get
   the t-shirt, i.e. the heaviest one.
*/
int xlshirt(int w1, int w2, int w3, int w4)
{
    int max;
    if(w1 > w2)
    max = w1;
    else 
    max = w2;
    
    if(max> w3)
    max = max;
    else
    max = w3;
    
    if(max > w4)
    max = max;
    else
    max = w4;
}

