#include <cstdio>
#include <iostream>
#include <vector>
#include <math.h> 
typedef long long int64;  

using namespace std;

//changing base without losing space
class cbwls
{
private:
	//precomputed stuff
	int n; // size of the array
	int sig; // size of the alphabet
	int block_size;
	int N; //number of blocks
	int64 X;
	vector <int64> Y,M,C;
	int64 sqX;
	int total_bits;
	double total_redundancy;
	double total_redundancy2;

	//change this data structure later
	vector <int64> data;


public:
	cbwls(int __n, int __sig)
	{
		n = __n;
		sig = __sig;

		block_size = 1+(int)(log2(n)/log2(sig));
		X = (int64) pow(sig,block_size);
		N = (n+block_size-1)/block_size;  //number of blocks
		sqX = (int)sqrt(X);
		total_bits = 0;
		total_redundancy = 0;
		total_redundancy2 = 0;

		Y.resize(N+1);
		M.resize(N+1);
		C.resize(N+1);

		Y[0] = 1;

		for(int i=0; i<N; i++)
		{
			int64 temp = Y[i]*sqX;

			while((1<<M[i]) < temp)
				M[i]++;

			C[i] = (1<<M[i])/Y[i];
			
			Y[i+1] =  (X+C[i]-1)/C[i];

			total_bits += M[i];
			total_redundancy += log2 ( 1/(1-(double)Y[i]/(1<<M[i])));
			total_redundancy2 += log2(1+(double)C[i]/X);

			// cout  <<i<<" Y "<< Y[i] <<" C "<< C[i] <<" M "<< M[i] <<" R1 " <<log2 ( 1/(1-(double)Y[i]/(1<<M[i]))) << " R2 " << log2(1+(double)C[i]/X) <<endl;
		}	

		cout << Y[N] << endl;
		//store just the spill in last 	
		while((1<<M[N]) < Y[N])
			M[N]++;

		total_bits += M[N];
		C[N] = 1;  //for the last block which stores only the final spill

		cout<<"expected " << int(n*log2(3))+1 << endl;
		cout << total_bits << endl;
		cout << total_redundancy << endl;
		cout << total_redundancy2 << endl;
		cout <<"percentage redundancy " <<  ( total_bits*1.0 - int(n*log2(3))+1 )*100/ (int(n*log2(3))+1) << endl;

		};

	~cbwls() {};

	void encode(vector <int>& A)
	{	
		data.resize(N+1);
		vector <int> x(N+1),y(N+1);
		
		y[0] = 0;

		for(int i=0;i<N+1;i++)
		{		
		if(i<N)	
			for(int j=0; j<block_size && i*block_size+j<n; j++)
			{
				x[i] *= sig;
				x[i] += A[i*block_size+j];
			}

			data[i] = (x[i]%C[i]) + y[i]*C[i];	
			y[i+1] = x[i]/C[i];
		}
		// cout<<"x[0] " << x[0] <<" data[0] "<<data[0]<< endl;
	}	

	//to access
	int operator [](int index)
	{
		int j = index/block_size;
		int64 temp = data[j]%C[j] + C[j]*(data[j+1]/C[j+1]);

			
		int u = ( (j<N-1 ? block_size : n%block_size) -1-(index%block_size));
		// cout<<temp<<' '<<u<<endl;
		
		for(int i=0; i<u; i++)
			temp /= sig;
		
		return temp%sig;
	} 	

	//to update...
};

int main() 
{
	int n = 10008;  
	int sig = 3;

	cbwls Ac(n,sig);

	//randomly generate the input within sigma
	vector <int> A(n);
	for(int i=0; i<n; i++)
		A[i] = rand()%sig;
	Ac.encode(A);

	//test by accessing Ac and see if it matches with A	
	for(int i=0;i<30;i++)
		cout<<A[i]<<' '<<Ac[i]<<endl;

}