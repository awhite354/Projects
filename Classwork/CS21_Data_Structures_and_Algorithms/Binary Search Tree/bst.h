/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 7 CS21
*/

#ifndef BST_H
#define BST_H

#include <iostream>

#include "node.h"

using namespace std;

class bst{
 public:
  bst();
  ~bst();
  void find(int);
  void insert(int);
  void Delete(int);
  void findMin();
  void findMax();
  void findPredecessor(int);
  void findSuccessor(int);
  void inOrder();
  void preOrder();
  void postOrder();
 private:
  Node* search(Node*,int);
  void insertNode(Node*);
  void deleteNode(Node*);
  Node* min(Node*);
  Node* max(Node*);
  Node* predecessor(Node*);
  Node* successor(Node*);
  void inorderTraversal(Node*);
  void preorderTraversal(Node*);
  void postorderTraversal(Node*);
  void destructorHelper(Node*);
  Node *root;
};
#endif
