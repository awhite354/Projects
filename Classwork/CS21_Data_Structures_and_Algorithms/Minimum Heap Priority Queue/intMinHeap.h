/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignemnt 2 CS21
 */

#ifndef INTMINHEAP_H
#define INTMINHEAP_H

#include <iostream>
#include <string>

using namespace std;

// integer minimum heap with PQ                                                            

class intMinHeap{
 public:
  intMinHeap(int);  // empty heap wth this capacity                                        
  ~intMinHeap();  // clean up allocated memory                                             
  std::pair<int *, int> heapsort();  // return sorted array and size from heap             
  string toString();
  bool heapinsert(int); // add element to heap; return success                             
  // min functions should return 0 in empty heaps                                          
  int minimum();  // return value of A[root]                                               
  int extractmin(); // return and remove A[root]                                           
  void decreasekey(int i, int k);  // A[i] decreased to k                                  
  bool isEmpty(){return size==0;}
  bool isFull();
 private:
  int minOf3(int, int, int); // with bounds check!                                         
  int left(int);
  int right(int);
  int parent(int);
  void buildheap();  // convert array to a heap                                            
  void heapify(int i);  // heapify at position i                                           
  int *A;  // array of integers - data                                                     
  int capacity; // size of array A                                                         
  int size; // data size in array                                                          
};

#endif
