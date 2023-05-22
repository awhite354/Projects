/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 1 CS21
*/


#include <iostream>
#include <string>
#include <fstream>

#include "queue.h"
#include "list.h"
#include "linknode.h"

using namespace std;

main(int argc, char *argv[]){

  int wordCount = 0;
  queue myQueue;
  ifstream infile(argv[1]);

  if(infile.is_open() && infile.good())
    {
      string line;
      while(getline(infile, line))
	{
	  myQueue.enqueue(line);
	  wordCount++;
	}
    }

  infile.close();

  while (wordCount > 0)
    {
      cout << myQueue.dequeue() << endl;
      wordCount--;
    }
  
  cout << endl;  
  
  return 0;
}
