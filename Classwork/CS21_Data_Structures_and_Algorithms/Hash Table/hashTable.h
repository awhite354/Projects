/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 5 CS21
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>

#include "hashEntry.h"

using namespace std;

class hashTable{
 public:
  hashTable(int size);
  void insert(int,string ); //insert data and key 
  string erase(int); //delete data from key
  string search(int); // returns string of data
  void clear();
  void hashToFile(string); //copies hash table to file
 private:
  int find(int); //returns element number in sub vector;
  int hash(int); //hash value for key
  vector<vector<hashEntry> > table; //vector of vectors storing the data
  int m; //table size
  static const double C = 0.618033988;
};

#endif
