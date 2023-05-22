/*                                                                                         
Anthony White                                                                              
awhite@pengo.cabrillo.edu                                                                  
anthywhite@gmail.com                                                                       
Assignment 1 CS21                                                                          
*/


#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>

#include "list.h"
#include "linknode.h"

using namespace std;

class queue
{
 private:
  list *listptr;
 public:
  queue();
  //~queue;
  void enqueue(string);
  string dequeue();
  bool isEmpty();

};

#endif
