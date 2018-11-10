#include <stdio.h>
#include <math.h>
#define RAND_MAX 32767
double log(double x)//we shall use Taylor's series to find out log
{
	if(x<0)
		return -1;
	int n=20;//if we take n=20, it kinda gives us a close estimation 
	double ans=0;
	int i;
	for(i=0;i<n;i++)
	{
		ans=ans+(pow(-1,i)*pow((x-1),(i+1))/(i+1));
	}

	return ans;
}
double pow(double a, double b)
{
	double ans=1;

	if(b==0)
		return 1;
	if(a==0)	
		return 0;

	if(b<0)
	{
		b=-1*b;
		a=1/a;
	}
	int i;
	for(i=1;i<=b;i++)
	{
		ans= ans*a;
	}

	return ans;
}
double expdev(double L)
{
	double val = 0.0;
	do
	{
		val = (double) rand()/RAND_MAX;
	}while(val ==0.0);
	val = -log(val )/L;
	return val;
}




