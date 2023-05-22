/*                                                                                         
Anthony White                                                                              
awhite@pengo.cabrillo.edu                                                                  
anthywhite@gmail.com                                                                       
Assignment 1 CS21                                                                          
*/


#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>

#include "linknode.h"

using namespace std;

class list{
  friend class queue;
 public:
  list();
  ~list();
  void insertFront(string);
  void insertRear(string);
  string removeFront();
  string removeRear();

 private:
  linknode *front;
  linknode *rear;
};

#endif
