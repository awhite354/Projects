/*                                                                                         
Anthony White                                                                              
awhite@pengo.cabrillo.edu                                                                  
anthywhite@gmail.com                                                                       
Assignment 1 CS21                                                                          
*/


#ifndef LINKNODE_H
#define LINKNODE_H

#include <iostream>
#include <string>

using namespace std;

class linknode
{
  friend class list;
 private:
  string data;
  linknode *next;
  linknode *prev;
  
 public:
  linknode();
};

#endif
