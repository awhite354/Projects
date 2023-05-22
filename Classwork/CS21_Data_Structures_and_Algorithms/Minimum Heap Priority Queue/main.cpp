/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assingment 2 
CS21
 */

#include "intMinHeap.h"

#include <utility>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string sortedArrayString(pair<int *, int>);

main(){
  int x;
  intMinHeap pQueue(2400000);
  while(cin >> x)
    {
      if(x > 0)
	{
	  pQueue.heapinsert(x);
	  cout << "insert: " << x << endl;
	}
      if(x == 0)
	{
	  cout << "heap size " << pQueue.toString() << endl;
	}
      if(x == -1)
	{
	  cout << "extract min: " << pQueue.extractmin() << endl;
	}
      if(x == -2)
	{
	  cout << "sorted array: " << sortedArrayString(pQueue.heapsort()) << endl;
	}
    }
  return 0;
}

string sortedArrayString(std::pair<int *, int> pair1){
  stringstream ss;
  string output;
  ss << '[';
  for(int i = 0; i < pair1.second; i++)
    {
      ss << pair1.first[i];
      if(i != pair1.second - 1)
	{
	  ss << ", ";
	}
    }
  ss << ']';
  output = ss.str();
  return output; 
}
