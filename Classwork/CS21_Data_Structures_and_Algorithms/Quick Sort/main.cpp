/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 3 Quicksort
CS21
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>

using namespace std;

void quickSort(vector<int>& A, int p, int r);
void quickSortH(vector<int>& A, int p, int r);
int partition(vector<int>& A, int p, int r);
int partitionH(vector<int>& A, int p, int r);
void medianOf3(vector<int>& A, int p, int r);

main(int argc, char *argv[]) {
  vector<int> A; 
  int x;
  while (cin >> x)
    {
      A.push_back(x);
    }
  if(argc > 1 && std::string(argv[1]) == "-h")
    {
      quickSortH(A, 0, A.size() - 1);
    }
    else
      {
	quickSort(A, 0, A.size()-1);
      }
  for(int i = 0; i < A.size() - 1; i++)
    {
      cout << setw(9) << setfill('0') << A[i] << endl;
    }
  return 0;
}

void quickSort(vector<int>& A, int p, int r){
  if(p < r)
    {
      int q = partition(A, p, r);
      quickSort(A, p, q-1);
      quickSort(A, q+1, r);
    }
}

void quickSortH(vector<int>& A, int p, int r){
  if(p < r)
    {
      int q = partitionH(A, p, r);
      quickSort(A, p, q-1);
      quickSort(A, p, r);
    }
}

int partition(vector<int>& A, int p, int r){
  if((r-p) > 5)
    {
      medianOf3(A, p, r);
    }
  int x = A[r];
  int i = p-1;
  for(int j = p; j < r; j++)
    {
      if(A[j] <= x)
	{
	  i++;
	  std::swap(A[i], A[j]);
	}
    }
  std::swap(A[i+1], A[r]);
  return (i+1);
}

int partitionH(vector<int>& A, int p, int r)
{
  if((r-p) > 5)
    {
      medianOf3(A, p, r);
    }
  int x = A[p];
  int i = p - 1;
  int j = r + 1;
  while(true)
    {
      do{j=j-1;} while(A[j] <= x);
      do{i=i+1;} while (A[i] >= x);
      if(i < j) {return j;}
      std::swap(A[i], A[j]);
    }
}

void medianOf3(vector<int>& A, int p, int r){
  int mid = (p+r)/2;
  if((p<mid && mid<r) || (r<mid && mid<p))
    {
      std::swap(A[mid], A[r]);
    }
  else if((mid<p && p<r) || (r<p && p<mid))
    {
      std::swap(A[p], A[r]);
    }
  //r must be the median and so no swap needed.
}
