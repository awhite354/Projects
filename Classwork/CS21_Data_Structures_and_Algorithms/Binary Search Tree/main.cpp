/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 7 CS21
*/

#include <iostream>
#include <sstream>

#include "bst.h"
#include "node.h"

using namespace std;

int main(){  
  bst myBst;
  string line;
  while(getline(cin, line))
    {  
      istringstream ss(line);
      bool comment = false;
      do {
      string word;
	  ss >> word;
	  if(word != "#")
	    {
	      if(word == "insert")
		{
		  int input;
		  ss >> input;
		  myBst.insert(input);
		}
	      if(word == "delete")
                {
                  int input;
                  ss >> input;
                  myBst.Delete(input);
                }
              if(word == "search")
                {
                  int input;
                  ss >> input;
                  myBst.find(input);
                }
              if(word == "successor")
                {
                  int input;
                  ss >> input;
                  myBst.findSuccessor(input);
                }
              if(word == "predecessor")
                {
                  int input;
                  ss >> input;
                  myBst.findPredecessor(input);
                }
              if(word == "min")
                {
		  myBst.findMin();
                }
              if(word == "max")
                {
                  myBst.findMax();
                }
	      if(word == "inorder")
                {
                  myBst.inOrder();
                }
              if(word == "postorder")
                {
                  myBst.postOrder();
                }
              if(word == "preorder")
                {
                  myBst.preOrder();
                }
	    }
	  if(word == "#")
	    {
	      comment = true;
	    }
	} while (ss && !comment);      
    } 
  return 0;
}
