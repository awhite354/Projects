/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 6 CS21
*/

#include <iostream>

#include "disjointSet.h"

using namespace std;

disjointSet::disjointSet(int n){
  setAmount = n*n;
  size = n*n;
  parent = new int[size];
  rank = new int[size];
  for(int i = 0; i < size; i++)
    {
      parent[i] = i;
      rank[i] = 0;
    }
}

disjointSet::~disjointSet(){
  delete parent;
  delete rank;
}

void disjointSet::Union(int x, int y){
  if(x < 0 || x >= size) return;
  if(y < 0 || y >= size) return;
  if(!sameSet(x,y))
    {
      Link(Find(x), Find(y));
      setAmount--;
    }  
  return;
}

int disjointSet::Find(int x){
  if(x < 0 || x >= size) return -1;
  if(x < size)
    {
      if(x != parent[x])
	{
	  parent[x] = Find(parent[x]);
	}
      return parent[x];
    }
}

int disjointSet::getSetAmount(){
  return setAmount;
}

bool disjointSet::sameSet(int x, int y)
{
  if(x < 0 || x >= size) return false;
  if(y < 0 || y >= size) return false;
  if(Find(x) == Find(y)) return true; //checks if paretn of x and y are the same.
  else return false;
}

void disjointSet::Link(int x, int y){
  if(x == y) return;
  if(rank[x] > rank[y])
    {
      parent[y] = x;
    }
  else
    {
      parent[x] = y;
      if(rank[x] == rank[y])
	{
	  rank[y]++;
	}
    }
  return;
}
