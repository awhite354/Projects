/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 4 Radix-Bucket Sort
CS21
*/

#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

const int bucketAmount = 10;

void radixBucketSort(vector<int>& A);
void bucketSort(int d, vector<int>& A);

main(){
  vector <int> A;
  int x;
  while (cin >> x)
    {
      A.push_back(x);
    }
  radixBucketSort(A);
  for(int i = 0; i < A.size(); i++)
    {
      cout << setw(9) << setfill('0') << A[i] << endl;
    }
  return 0;
}

void radixBucketSort(vector<int>& A)
{
  for(int d = 1; d < pow(10, bucketAmount-1); d = d*10)
    {
      bucketSort(d, A);
    }
  return;
}

void bucketSort(int d, vector<int>& A)
{
  int preserveSize = A.size();
  vector <int> Bucket[bucketAmount];
  for(int i = 0; i < preserveSize; i++)
    {
      int digit = A[i]/d%10;
      Bucket[digit].push_back(A[i]);
    }
  A.clear();
  int counter = 0;
  for(int j = 0; j < bucketAmount; j++)
    {
      for(int k = 0; k < Bucket[j].size(); k++)
	{
	  A.push_back(Bucket[j][k]);
	  counter++;
	}
    }
  return;
}
