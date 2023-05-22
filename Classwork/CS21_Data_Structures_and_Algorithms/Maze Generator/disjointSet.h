/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 6 CS21
*/

#ifndef DISJOINTSET_H
#define DISJOINTSET_H

using namespace std;

class disjointSet{
 public:
  disjointSet(int); //constructor creates n number of sets.
  ~disjointSet(); //destructor deletes pointers.
  void Union(int, int); //Unions two sets optimizing by rank.
  int Find(int); //returns parent of parameter given to it.
  int getSetAmount(); //returns the amount of sets.
  bool sameSet(int, int); // returns whetherx & y are in the same set.
 private:
  void Link(int, int); //links the parents of the sets together.
  int setAmount;
  int size;
  int *parent;
  int *rank;
};
#endif
