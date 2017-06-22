#include <cstdio>
#include <iostream>
#include <vector>
#include <math.h> 
typedef long long int64;  

using namespace std;

//changing base without losing space (section 4.2)

/* 
This implementaion encodes a string of size sigma with alphabet of size n into an encoding which 
takes o(1) space extra than the theoretical lower bound. Also it uses O(log n) precomputed constants.
*/


class cbwls
{
private:
	//precomputed stuff
	int n; // size of the array
	int sig; // size of the alphabet
	int block_size;
	int N; //number of blocks
	int height;

	vector <int64> Y[3],M[3],C[3],S[3];

	//change this one later	
	vector <int64> data;

public:
	//gives the class of the node 
	int get_class(int i)
	{	
		if(i>N) return -1;

		int cur_height = (int)log2(i);
		int subtree_height = (int)log2(n/i);

		if(n < pow(2,subtree_height)*(i+1)-1)
			return 2;
		else if(subtree_height == height - cur_height)
			return 0;
		else
			return 1;
	}

	cbwls(int __n, int __sig)
	{
		n = __n;
		sig = __sig;
		block_size = 1+(int)(log2(n)/log2(sig));
		N = (n+block_size-1)/block_size;  //number of blocks
		int64 X = (int64) pow(sig,block_size);
		int64 sqX = (int)sqrt(X);
		height = (int)log2(N);  //need to be optimized at the query time!!
		int cur_height;
		int clas;
		
		for(int i=0;i<3;i++)
		{	
			Y[i].resize(height+1);
			M[i].resize(height+1);
			C[i].resize(height+1);
			S[i].resize(height+1);
		}	

		for(int i=N; i>=1; i--)
		{
			clas = get_class(i);
			cur_height = (int)log2(i);

			if(Y[clas][cur_height]!=0) continue;

			int clas_c1 = get_class(2*i);
			int clas_c2 = get_class(2*i+1);
			

			Y[clas][cur_height] = (clas_c1!=-1 ? S[clas_c1][cur_height+1] : 1)* 	
								(clas_c2!=-1 ? S[clas_c2][cur_height+1] : 1);						
			
			int64 temp = Y[clas][cur_height]*sqX;

			while((1<<M[clas][cur_height]) < temp)
				M[clas][cur_height]++;

			C[clas][cur_height] = (1<<M[clas][cur_height])/Y[clas][cur_height];
			//storing the spill
			S[clas][cur_height] = (X+C[clas][cur_height]-1)/C[clas][cur_height];
		}

		//something must be done with the spill of 1!
	}

	~cbwls() {};

	void encode(vector <int>& A)
	{	
		data.resize(N+1);
		vector <int> x(N+1),y(N+1),s(N+1);
		int cur_height;
		int clas;

		for(int i=N; i>=1; i--)
		{		
		cur_height = (int)log2(i);	
		clas = get_class(i);		

		for(int j=(i-1)*block_size; j<i*block_size && j<n; j++)
			{
				x[i] *= sig;
				x[i] += A[j];
			}

			y[i] = (2*i<=N ? s[2*i] : 0) + 
				(2*i+1<=N ? S[get_class(2*i)][cur_height+1]*s[2*i+1] : 0);

			data[i] = (x[i]%C[clas][cur_height]) + y[i]*C[clas][cur_height];	
			s[i] = x[i]/C[clas][cur_height];
		}

		// cout<<s[3]<<endl;

		data[0] = s[1]; 
	}	

//to access
	int operator [](int index)		
	{
		int j = index/block_size+1;
		int cur_height = (int)log2(j);	
		int clas = get_class(j);	

		int yd;

		if(j == 1) yd = data[0];
		else
		{	
			yd  = (data[j/2]/C[get_class(j/2)][cur_height-1]);

			if(j%2==1)
				yd /= S[clas][cur_height];
			else
				yd %= S[get_class(j-1)][cur_height];
		}

		int64 temp = data[j]%C[clas][cur_height] + 
			C[clas][cur_height]*yd;
			
		int u = ( (j<N-1 ? block_size : n%block_size) -1-(index%block_size));
		
		// cout<<temp<<' '<<yd<<' '<<C[clas][cur_height]<<endl;
		
		for(int i=0; i<u; i++)
			temp /= sig;

		return temp%sig;
	}
//to update

};


int main() 
{
	int n = 100;  
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
	return 0;

}