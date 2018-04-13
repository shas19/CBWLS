#include <bits/stdc++.h>
using namespace std;
// use n+2*w bits
// b = 16 bits  

const int MAX = (int)1e4; 
int A[MAX];
const int b = 16;
const int b2 = 32;


class D1
{	
	int N;
	int k;

public:
	D1(int __n)
	{
		N = __n/32;
		k = N;

		// cout << N << ' ' << k <<' ' <<__n<< endl;
	}

	int uh(int x)
	{
		return x&((1<<16)-1);
	}

	int lh(int x)
	{
		return (x>>16);
	}

	void setl(int i, int k)
	{
		A[i] = (k<<16) | (uh(A[i]));
	}

	void seth(int i, int k)
	{
		A[i] = (lh(A[i]) << 16) | k;
	}

	int join(int a,int b)
	{
		return (a<<16)|b;
	}

	int mate(int s)
	{
		int t = uh(A[s]);

		if( ((1<=s and s<=k and k < t and t <= N) or (1 <= t and t <= k and k<s and s<=N)) 
			and uh(A[t]) == s)
			return t;
		else
			return s;
	}
	
	int read(int i)
	{
		if(mate(i) <= k)
			return 0;
		if(i > k)
			return A[i];
		else
			return join(lh(A[i]), lh(A[mate(i)]));
	}

	int simple_write(int i, int x)
	{
		if(i<=k)
		{
			setl(i,lh(x));
			setl(mate(i),uh(x));
		}
		else
		{
			A[i] = x;
			int j = mate(i);
			if(i!=j)
				seth(j,j);
		}	
	}

	int fnz(int s)
	{	
		// cout << s<<' ' << read(s) << endl;
		int u = read(s);

		for(int i=0;i<32;i++) 
			if(u&(1<<i))
				return i;
		return 0;	
	}

	int nonzero()
	{	
		// cout <<"L "<< k << endl;
		if(k == N) 
			return 0;
		else 
			return mate(N);
	}

	int write(int i, int x)
	{
		int x0 = read(i);
		int id = mate(i);

		if(x != 0)
		{
			if(x0 == 0)
			{
				int kd = mate(k);
				int u = read(k);
				k = k-1;
				simple_write(k+1,u);

				if(i!= kd)
				{
					seth(id,kd);
					seth(kd,id);
					setl(kd,lh(A[i]));
				}	
			}

			simple_write(i,x);	
		}	
		else
		{
			if(x0 != 0) //a deletion
			{
				int kd = mate(k+1);
				int v = read(kd);
				k = k+1;
				seth(id,kd);
				seth(kd,id);

				if(kd!=i) 
					simple_write(kd,v);
			}	
		}	

	}

};


class D2
{
	int sz;
	vector <bool> A;

public:
	D2(int __sz)
	{
		sz = __sz;
		A.resize(sz);
	}

	void insert(int s)
	{	
		A[s-1] = 1;
	}

	void remove(int s)
	{
		A[s-1] = 0;
	}


	int choice()
	{
		for(int i=0;i<sz;i++)
			if(A[i]!=0)
				return i+1;
		return 0;	
	}

	bool contains()
	{
			for(int i=0;i<sz;i++)
				if(A[i]!=0)
					return 1;
		return 0;	
	}	

};

//for now OCD works only if n is divisible by 2b
class OCD
{
	int n;
	D1* A;
	D2* B;

public:
	OCD(int __n)
	{
		n = __n;
		A = new D1((n/b2)*b2);
		B = new D2(n%b2); //do not test D2 for now
	}


	void insert(int s)
	{
		int p,q;
		p = s/b2+1;
		q = s%b2;

		int t = A->read(p);
		t = t|(1<<q);

		A->write(p,t);
	}

	void remove(int s)
	{	
		int p,q;
		p = s/b2+1;
		q = s%b2;

		int t = A->read(p);

		// cout << "b "<<t<<endl;
		t = t&(~(1<<q));
		// cout << "b "<<t<<endl;

		A->write(p,t);

	}

	bool contains(int s)
	{	
		int p,q;
		p = s/b2+1;
		q = s%b2;

		int t = A->read(p);
		if(t&(1<<q))
			return true;
		else
			return false;
	}

	int choice()
	{
		int nz = A->nonzero();
		// cout << nz << endl;
		if(nz == 0)
			return 0;
		
		return (nz-1)*b2 + A->fnz(nz);
	}
};



int main()
{
	OCD* A;
	int n = 64;
	A = new OCD(n);

	//test 

	A->insert(10);
	A->remove(10);
	A->insert(5);
	A->insert(7);

	cout << A->contains(5) << endl;
	cout << A->contains(7) << endl;
	cout << A->contains(8) << endl;
	cout << A->contains(9) << endl;
	cout << A->choice() << endl;


	A->insert(62);
	A->insert(33);
	A->insert(35);

	cout << A->contains(62) << endl;
	cout << A->contains(33) << endl;
	cout << A->contains(35) << endl;
	cout << A->contains(37) << endl;
	cout << A->choice() << endl;


}
