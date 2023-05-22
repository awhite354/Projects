/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 6 CS21
*/

#include <algorithm>
#include <iostream>
#include <time.h>

#include "maze.h"

using namespace std;

maze::maze(int n){
  dim = n;
  size = n*n;
  randOrderArray = new int[n*n];
  disSetPtr = new disjointSet(n);
  mazePtr = new int[n*n];
  mazePtr[0] = 11; //sets the beginning square
  for(int i = 1; i < n*n; i++)
    {
      randOrderArray[i] = i;
      mazePtr[i] = 15;
    }
  mazePtr[n*n-1] = 14; //sets the last square
}

maze::~maze(){
  delete mazePtr;
  delete disSetPtr;
  delete randOrderArray;
}

void maze::randomizeArray(){
  random_shuffle(randOrderArray, randOrderArray + size);
  return;
}

void maze::mazeGenerate(){
  randomizeArray();
  srand (time(0)); //sets random seed with the time.
  int tag;
  while(disSetPtr -> getSetAmount() > 1)
    {
      for(int i = 0; i < size; i++)
	{
	  tag = rand() % 4; //randomly chooses an neighbor to pick.
	  if(tag == 0)
	    {
	      breakRight(randOrderArray[i]);
	    }
	  if(tag == 1)
            {
	      breakLeft(randOrderArray[i]);
	    }
	  if(tag == 2)
            {
	      breakTop(randOrderArray[i]);
	    }
	  if(tag == 3)
            {
	      breakBottom(randOrderArray[i]);
	    }
	}
    }
  return;
}

void maze::breakRight(int x){
  if(x % dim == dim - 1 || disSetPtr -> sameSet(x, x+1) || x+1 >= size)
    {
      return;
    }
  else //breaks down both x's wall and its neighbors wall.
    {
      mazePtr[x] = mazePtr[x] - 1;
      mazePtr[x+1] = mazePtr[x+1] - 4;
      disSetPtr->Union(x, x+1);
    }
  return;
}

void maze::breakLeft(int x){
  if(x % dim == 0 || disSetPtr -> sameSet(x, x-1) || x-1 < 0)
    {
      return;
    }
  else //breaks down both x's wall and its neighbors wall.   
    {
      mazePtr[x] = mazePtr[x] - 4;
      mazePtr[x-1] = mazePtr[x-1] - 1;
      disSetPtr->Union(x, (x-1));
    }
  return;
}

void maze::breakTop(int x){
  if(x < dim || disSetPtr -> sameSet(x, x-dim) || x-dim < 0)
    {
      return;
    }
  else //breaks down both x's wall and its neighbors wall.   
    {
      mazePtr[x] = mazePtr[x] - 8;
      mazePtr[x-dim] = mazePtr[x-dim] - 2;
      disSetPtr->Union(x, x-dim);
    }
  return;
}

void maze::breakBottom(int x){
  if(x >= (size - dim) || disSetPtr -> sameSet(x, x+dim) || x+dim >=size)
    {
      return;
    }
  else //breaks down both x's wall and its neighbors wall.   
    {
      mazePtr[x] = mazePtr[x] - 2;
      mazePtr[x+dim] = mazePtr[x+dim] - 8;
      disSetPtr->Union(x, x+dim);
    }
  return;
}

void maze::printMaze(){ //uses a look up array to print maze in correct format.
  char lookUp[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  for(int i = 0; i < size; i++)
    {
      cout << lookUp[mazePtr[i]];      
      if(i % dim == dim - 1)
	{
	  cout << endl;
	}
    }
  cout << endl;
  return;
}
