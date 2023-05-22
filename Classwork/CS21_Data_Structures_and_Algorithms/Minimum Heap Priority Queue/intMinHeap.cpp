/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assingment 2 CS21    
 */

#include "intMinHeap.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

intMinHeap::intMinHeap(int i) {
  capacity = i;
  size = 0;
  A = new int[capacity];  
}

intMinHeap::~intMinHeap() {
  delete [] A;
  A = NULL;
}

std::pair<int *, int> intMinHeap::heapsort() { 
  int oldSize = size;
  int *B = new int[size];
  int *temp;

  for(int i = 0;i < size; i++)
    {
      B[i] = A[i];
    }
  temp = A;
  A = B;

  for(int i = size - 1; i > 0; i--)
    {
      std::swap(A[0],A[i]);
      size--;
      heapify(0);
    }
  A = temp;
  size = oldSize;
  return std::make_pair (B,oldSize);
}

string intMinHeap::toString() {
  stringstream ss;
  string output;
  ss << size << ": ";
  for(int i = 0; i < size; i++)
    {
      ss << A[i];
      if(i != size - 1)
	{
	  ss << ", ";
	}
    }
  output = ss.str();
  return output;
}

bool intMinHeap::heapinsert(int key) {
  if(!isFull())
    {
      size++;
      A[size-1] = key + 1;
      decreasekey(size - 1, key);
    }
  else
    {
      return false;
    }
}

int intMinHeap::minimum() {
  return A[0];
}

int intMinHeap::extractmin() {
  if(size == 0)
    {
      return -1;
    }
  int min = A[0];
  A[0] = A[size-1];
  size--;
  heapify(0);
  return min;
}

void intMinHeap::decreasekey(int i, int key) {  
  if (i < size)
    {
      if(key >= A[i])
	{
	  return;
	}
      A[i] = key;
      while(i > 0 && A[parent(i)] > A[i])
	{
	  std::swap(A[i],A[parent(i)]);
	  i = parent(i);
	}
    }
  return;
}

bool intMinHeap::isFull() {
  if(size == capacity)
    {
      return true;
    }
  else
    {
      return false;
    }
}

int intMinHeap::minOf3(int i, int j, int k) {
  int min = i;
  if(j < size && A[j] < A[min])
    {
      min = j;
    }
  if(k < size && A[k] < A[min])
    {
      min = k;
    }
  return min;
}

int intMinHeap::left(int i) {
  return ((2*i)+1);
}

int intMinHeap::right(int i) {
  return ((2*i)+2);
}

int intMinHeap::parent(int i) {
  return ((i-1)/2);
}

void intMinHeap::buildheap() {
  for(int i = size/2 - 1; i >= 0; i--)
    {
      heapify(i);
    }
}

void intMinHeap::heapify(int i) {
  int n = minOf3(i, left(i), right(i));
  if(i != n)
    {
      std::swap(A[i],A[n]);
      heapify(n);
    }
}
