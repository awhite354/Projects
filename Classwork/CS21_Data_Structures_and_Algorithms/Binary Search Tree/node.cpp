/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 7 CS21
*/

#include <iostream>

#include "node.h"

using namespace std;

Node::Node(int k){
  //constructor
  key = k;
  parent = NULL;
  left = NULL;
  right = NULL;
}

