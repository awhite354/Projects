/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 5 CS21
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "hashTable.h"
#include "hashEntry.h"

using namespace std;

std::pair<int, string> stringSplitter(string input);

int main(){

  int size = 178000;
  hashTable myHashTable(size);
  int choice;
  string menu = "(1)load (2)insert (3)delete (4)search (5)clear (6)save (7)quit -- Your choice? ";
  cout << menu;
  cin >> choice;
  while(choice > 0 && choice < 7)
    { 
      if(choice == 1){
	string filename;
	cout << "read hash table - filename? ";
	cin >> ws;
	getline(cin, filename);	
	ifstream infile(filename.c_str());
	if(!infile)
	  {
	    cout << "File couldn't be opened." << endl; 
	  }
	else
	  {
	    string line;
	    while(getline(infile, line))
	      {
		pair<int, string> p = stringSplitter(line);
		myHashTable.insert(p.first, p.second);
	      }
	  }
      }
      if(choice == 2){
	string readInput;
	cout << "insert new record: \n";
	cin >> ws;
	getline(cin, readInput);
	pair<int, string> p = stringSplitter(readInput);
	myHashTable.insert(p.first, p.second);
      }
      if(choice == 3){
	cout << "delete record - key? ";
	int key;
	cin >> key;
	string data = myHashTable.erase(key);
	if(data == "")
	  {
	    cout << "Delete not found: " << key << endl;
	  }
	else
	  {
	    cout << "Delete: " << key << data << endl;
	  }
      }
      if(choice == 4){
	cout << "search record - key? ";
	int key;
	cin >> key;
	string data = myHashTable.search(key);
	if(data == "")
	  {
	    cout << "Search not found: " << key << endl;
	  }
	else
	  {
	    cout << "Found: " << key << data << endl;
	  }
      }
      if(choice == 5){
	myHashTable.clear();
	cout << "clearing hash table." << endl;
      }
      if(choice == 6){
	string filename;
	cout << "write hash table - filename? ";
	cin >> ws;
	getline(cin, filename);
	myHashTable.hashToFile(filename);	
      }
      cout << menu;
      cin >> choice;
    }
  return 0;
}

std::pair<int, string> stringSplitter(string input){
  int keyInt;
  string keyStr = input.substr(0,9);
  stringstream s(keyStr);
  s >> keyInt;
  string dataStr = input.substr(9);
  return std::make_pair(keyInt, dataStr);
}
