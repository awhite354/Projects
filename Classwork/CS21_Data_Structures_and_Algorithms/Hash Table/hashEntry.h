/*                                                
Anthony White                                                                             
awhite@pengo.cabrillo.edu                                                                 
anthywhite@gmail.com                                                                      
Assignment 5 CS21                                                                         
*/

#ifndef HASHENTRY_H
#define HASHENTRY_H
 

#include <iostream>
#include <string>

using namespace std;

class hashEntry{
  friend class hashTable;
 public:
  hashEntry(int key, string data);
  int getKey();
  string getData();
 private:
  int key;
  string data;
};

#endif
