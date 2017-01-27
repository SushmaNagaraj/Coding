/*
   Implement a function called boxInfo that computes the surface 
   area AND the volume of a box. 
   Choose appropriate types for the parameters and
   the return value.
   @param a, b, c the side lengths
   @param surfaceArea set to the surface area
   @param volume set to the volume
*/
void boxInfo(double length,double breadth,double height,double& result1, double& result2)
{
    result1 = 2 * length * breadth + 2 * breadth * height + 2 * height * length;  
    result2 = length * breadth * height;
}