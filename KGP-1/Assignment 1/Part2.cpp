#include<bits/stdc++.h>
using namespace std;

void hashing(int H[], int size, int HX[] , int len)
{
	int p = 7, b = 3;

	int h = 0,i,x = size - len;
	


	for(i = 0 ; i < len ; ++i)
	{
		HX[0] = ((H[i]  + h) * b) % p;					//creating window 1
	}
	
	for(i = 0 ; i < )

	for(i = 0 ; i < x ; ++i)
	{

		HX[i+1] = ( ( ( HX[i] -( ( H[i] * pow(b,len-1) ) % p ) * b ) % p ) +  ( H[i + len] % p ) ) mod p 			//rolling the window

	}

	for(i = 0 ; i <= x ; ++i)
		cout<<HX[i]<<endl;
}

int main()
{
	int H[3] = {1,2,3} ,alpha[3] = {0,0,0}, len = 3;
	hashing(H,3,alpha,3);

	
}

