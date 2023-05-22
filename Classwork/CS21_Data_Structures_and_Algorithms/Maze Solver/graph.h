/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 8 CS21
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>

using namespace std;

class graph {
 public:
  graph(int);
  void BFS(vector<int>, int);
  vector <int> findAdjacent(int, int);
  void pathFinder();
 private:
  int dim;
  int size;
  vector<int> d;
  vector<string> color;
  vector<int> p;
};

#endif
