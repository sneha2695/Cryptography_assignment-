/*Program for 2nd assignment */

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <math.h>

double findSQRT(double N) { return sqrt(N); }

// Function to return gcd of 2 numbers 
int gcd(int a, int b)
{
    if (a == 0)
        return b;
    return gcd(b%a, a);
}



// Driver program to test above function
int main()
{
    double phi ,probabilty,value;
	int num=0,n=0,count=0 ,m,r;
	int random[1000];
	int gcd1;
	printf("Enter the num of random numbers");
	scanf("%d",&num);
	srand(time(NULL));
	for(int i=0;i<num;i++)
	{
	  random[i]=rand()% 100 + 1;
	}
	  
   for (int j = 0; j < num; j+=2)
   {
      m = random[j];
      n = random[j + 1];
      gcd1= gcd(m,n);
      if (gcd1 == 1)
	  {
        count = count + 1;
	  }
   }
	n=num/2;
	probabilty = (double)count/n ;
	printf("Value of probabilty %f\n",probabilty);
	value = 6/probabilty ;
	
	phi = findSQRT(value);
	
	printf("Value of ph %f\n",phi);
	
	return 0;
	
}