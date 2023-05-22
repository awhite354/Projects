/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 7 CS21
*/

#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;


class Node{
  friend class bst;
 public:
  Node(int);
 private:
  int key;
  Node *parent;
  Node *left;
  Node *right;
};
#endif
