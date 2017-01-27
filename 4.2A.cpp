//SOLUTION
/**
   Return a pointer to the first negative element in 
   the given array which has length n. If there is no negative element,
   return a pointer past the last element. 
*/
int* firstneg(int* arr, int n)
{
    int temp=0;
    int *result = &arr[n];;
    for(int i=0;i<n;i++){
        if(arr[i] < temp){
            result = &arr[i];
            break;
        }
        else 
        continue;
    }
    return result;
}

#include <iostream>

using namespace std;

int* firstneg(int* arr, int n);

int main()
{
   int a[] = { 1, 4, 9, -4, 8, -9, 0 };
   int* r = firstneg(a, sizeof(a) / sizeof(a[0]));
   cout << hex << r << endl;
   cout << "Expected: " << a + 3 << endl;

   int b[] = { 1, 4, 9, 0 };
   r = firstneg(b, sizeof(b) / sizeof(b[0]));
   cout << hex << r << endl;
   cout << "Expected: " << b + 4 << endl;

   r = firstneg(a, 0);
   cout << hex << r << endl;
   cout << "Expected: " << a << endl;
   
   return 0;
}




