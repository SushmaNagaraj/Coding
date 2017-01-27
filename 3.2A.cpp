/**
   Computes the average of all positive elements in the given array.
   @param a an array of integers
   @param alen the number of elements in a
   @return the average of all positive elements in a, or 0 if there are none.
*/
double avgpos(int a[], int alen)
{
  int count =0;
  int sum = 0;
  double average =0;
  for(int i=0;i<alen;i++){
	if(a[i]>0){
	sum = sum + a[i];
	count++;
	}
  }
  if(count>0 && sum>0){
   average = sum/count;
  }
  return average;
}

