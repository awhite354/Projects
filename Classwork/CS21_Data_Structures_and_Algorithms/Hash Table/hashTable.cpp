/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 5 CS21
*/

#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>


#include "hashTable.h"
#include "hashEntry.h"

using namespace std;

hashTable::hashTable(int size){
  m = size;
  table.resize(size);
}

int hashTable::hash(int key){
  return floor(m*(key*C-floor(key*C)));
}

void hashTable::insert(int key, string data){
  int hashValue = hash(key);
  table[hashValue].push_back(hashEntry(key, data));
  return;
}

string hashTable::erase(int key){
  int hashValue = hash(key);
  if(find(key) != -1)
    {
      string data = table[hashValue][find(key)].getData();
      std::swap(table[hashValue][find(key)], table[hashValue][table[hashValue].size()-1]);
      table[hashValue].pop_back();
      return data;
    }
  return "";
}

string hashTable::search(int key){
  int hashValue = hash(key);
  for(int i = 0; i < table[hashValue].size(); i++)
    {
      if(table[hashValue][i].getKey() == key)
	{
	  return table[hashValue][i].getData();
	}
    }
  return "";
}

void hashTable::clear(){
  int i = 0;
  while(i < m)
    {
      while(table[i].size() != 0)
	{
	  table[i].pop_back();
	}
      i++;
    }
  return;
}

void hashTable::hashToFile(string filename){
  int i = 0;
  ofstream outFile(filename.c_str());
  while(i < m)
    {
      if(table[i].size() != 0)
	{
	  for(int j = 0; j < table[i].size(); j++)
	    {
	      outFile << table[i][j].getKey() << table[i][j].getData() << endl;
	    }
	}
      i++;
    }
  return;
}

int hashTable::find(int key){
  int hashValue = hash(key);
  for(int i = 0; i < table[hashValue].size(); i++)
    {
      if(table[hashValue][i].getKey() == key)
	{
	  return i;
	}
    }
  return -1;
}


