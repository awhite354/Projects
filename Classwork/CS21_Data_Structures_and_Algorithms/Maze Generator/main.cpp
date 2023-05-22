/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 5 CS21
*/

#include <cstdlib>
#include <iostream>
#include <string>

#include "disjointSet.h"
#include "maze.h"

using namespace std;

int main(int argc, char* argv[]){
  int n;
  if(argc == 1)
    {
      cout << "Error: You must enter the maze dimension in the command line." << endl; 
      return 0;
    }
  if(argc == 2)
    {
      n = atoi(argv[1]);
    }
  if(argc >= 3)
    {
      cout << "Error: Too many command line parameters." << endl;
      return 0;
    }
  if(n < 3)
    {
      cout << "Error: Maze dimension must be greater than three." << endl;
      return 0;
    }
  maze myMaze(n);
  myMaze.mazeGenerate();
  myMaze.printMaze();
  return 0;
}
