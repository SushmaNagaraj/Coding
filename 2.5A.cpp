/**
   We have three t-shirts in sizes small, medium, and large.
   Write a function that takes the weights of three people
   and sets them so that the first parameter becomes
   the weight of the person who should get the small shirt
   (i.e. the lightest one), the second one becomes the weight 
   of the person who should get the medium one (i.e. the
   second-lightest one), and the third parameter becomes
   the weight of the person who should get the largest one.
*/
void smlShirts(int& first, int& second, int& third)
{
int temp;
  if(first < second){
	  if( first < third){
		  first = first;
	 }else {
			temp = first;
			first = third;;
			third = temp;
	  }
	  if(second < third){
		  second = second;
	  }else{
		  temp = second;
		  second = third;
		  third = temp;
	  }
  }else {
	   temp = first;
	   first = second;
       second = temp;		

	   if( first > third){
		   temp = first;
		   first = third;
		   third = temp;
	   }
	   if (second > third){ 
		   temp = second;
		   second = third;
           third = temp; 
	   }else{ 
		   second = second;
		   third = third;
	   } 
   }
}
