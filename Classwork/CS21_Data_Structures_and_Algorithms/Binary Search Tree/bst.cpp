/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 7 CS21
*/

#include <iostream>

#include "bst.h"

using namespace std;

bst::bst()
{  
  root = NULL;
}

bst::~bst()
{
  if(root)
    {
      destructorHelper(root); 
    }
}

void bst::destructorHelper(Node *x){
  if(x != NULL)
    {
      if(x -> left != NULL)
	{
	  destructorHelper(x -> left);
	}
      if(x -> right != NULL)
	{
	  destructorHelper(x -> right);
	}
      delete x;
    }
  return;
}

void bst::find(int key){
  Node* temp = search(root, key);
  if(temp == NULL)
    {
      cout << key << " not found." << endl;
    }
  else if(temp != NULL)
    {
      cout << key << " found." << endl;
    }
}

Node* bst::search(Node *x,int key){
  if(x == NULL || x -> key == key)
    {
      return x;
    } 
  if(key < x -> key)
    {
      return search(x -> left, key);
    }
  return search(x -> right, key);
    
}

void bst::insert(int key){
  Node *temp = new Node(key);
  insertNode(temp);
  cout << "inserted " << key << "." << endl;
}

void bst::insertNode(Node *x){
  Node *Parent = NULL;
  Node *temp = root;
  while(temp != NULL)
    {
      Parent = temp;
      if(x -> key < temp -> key)
	{
	  temp = temp -> left;
	}
      else
	{
	  temp = temp -> right;
	}
    }
  x -> parent = Parent;
  if(Parent == NULL)
    {
      root = x; return;
    }
  else if(x -> key < Parent -> key)
    {
      Parent -> left = x;
    }
  else
    {
      Parent -> right = x;
    }
  return;
}

void bst::Delete(int key){
  if(search(root, key) == NULL)
    {
      cout << "delete " << key << " - not found." << endl;
      return;
    }
  deleteNode(search(root, key));
  cout << "deleted " << key << "." << endl;
}

void bst::deleteNode(Node *n){
  Node *temp;
  Node *x;
  if(n -> left == NULL || n -> right == NULL)
    {
      temp = n;
    }
  else
    {
      temp = successor(n);
    }
  if(temp -> left != NULL)
    {
      x = temp -> left;
    }
  else
    {
      x = temp -> right;
    }
  if( x != NULL)
    {
      x -> parent = temp -> parent;
    }
  if(temp -> parent == NULL)
    {
      root = x;
    }
  else if(temp == temp -> parent -> left)
    {
      temp -> parent -> left = x;
    }
  else
    {
      temp -> parent -> right = x;
    }
  if(temp != n)
    {
      n -> key = temp -> key;
    }
  delete temp;
}

void bst::findMin(){
  Node* temp = min(root);
  cout << "min is " << temp -> key << "." << endl;
  return;	   
}

Node* bst::min(Node* x){
  while(x -> left != NULL)
    {
      x = x -> left;
    }
  return x;
}

void bst::findMax(){
  Node* temp = max(root);
  cout << "max is " << temp -> key << "." << endl;
}

Node* bst::max(Node* x){
  while(x -> right != NULL)
    {
      x = x -> right;
    }
  return x;
}

void bst::findPredecessor(int key){
  if(search(root, key) == NULL)
    {
      cout << key << " not in tree." << endl;
      return;
    }
  Node* temp = predecessor(search(root, key));
  if(temp  == NULL)
    {
      cout << "no predecessor for " << key << "." << endl;
    }
  else
    {
      cout << key << " predecessor is " << temp -> key << "." << endl;
    }
}

Node* bst::predecessor(Node *x){
  if(x -> left != NULL)
    {
      return max(x -> left);
    }
  Node *temp = x -> parent;
  while(temp != NULL && x == temp -> left)
    {
      x = temp;
      temp = temp -> parent;
    }
  return temp;
}

void bst::findSuccessor(int key){
  if(search(root, key) == NULL)
    {
      cout << key << " not in tree." << endl;
      return;
    }
  Node* temp = successor(search(root, key));
  if(temp  == NULL)
    {
      cout << "no successor for " << key << "." << endl;
    }
  else{
    cout << key << " successor is " << temp -> key << "." << endl;
  }
}


Node* bst::successor(Node *x){
  if(x -> right != NULL)
    {
      return min(x -> right);
    }
  Node *temp = x -> parent;
  while(temp != NULL && x == temp -> right)
    {
      x = temp;
      temp = temp -> parent;
    }
  return temp;
}

void bst::inorderTraversal(Node *x){
  if(x == NULL) return;
  inorderTraversal(x -> left);
  cout << x -> key << " "; //print x 
  inorderTraversal(x -> right);
  return;
}

void bst::preorderTraversal(Node *x){
  if(x == NULL) return;
  cout << x -> key << " "; //print x
  preorderTraversal(x -> left);
  preorderTraversal(x -> right);
  return;
}

void bst::postorderTraversal(Node *x){
    if(x == NULL) return;
    postorderTraversal(x -> left);
    postorderTraversal(x -> right);
    cout << x -> key << " "; //print x
    return;
}

void bst::inOrder(){
  cout << "inorder traversal:" << endl;
  inorderTraversal(root);
  cout << endl;
}

void bst::preOrder(){
  cout << "preorder traversal:" << endl;
  preorderTraversal(root);
  cout << endl;
}

void bst::postOrder(){
  cout << "postorder traversal:" << endl;
  postorderTraversal(root);
  cout << endl;
}

