/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 5 CS21
*/


#include <iostream>
#include <string>

#include "hashEntry.h"

using namespace std;

hashEntry::hashEntry(int Ikey, string Idata){
  key = Ikey; 
  data = Idata;
}

int hashEntry::getKey(){
  return key;
}

string hashEntry::getData(){
  return data;
}
