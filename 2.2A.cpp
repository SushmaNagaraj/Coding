/*
   Implement a function called surfaceArea that computes the surface 
   area of a box. Choose appropriate types for the parameters and
   the return value.
   @param a, b, c the side lengths
   @return the surface area
*/
double surfaceArea(double a, double b, double c)
{
   double surfaceArea = 2 * a * b + 2 * b * c + 2 * c *a;
   return surfaceArea;
}