/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 8 CS21
*/

#include <iostream>
#include <string>
#include <vector>

#include "graph.h"

using namespace std;

int main(){
  vector<int> maze;
  string input;
  int dim;
  int size;
  while (cin >> input)
    { 
      dim = input.length();
      for(int i = 0; i < dim; i++)
	{
	  maze.push_back(input[i]);
	}
    }
  size = dim*dim;
  for(int j = 0; j < size; j++)
    {
      if(maze[j] >= 97)
	{
	  maze[j] = maze[j] - 87;
	}
      else if(maze[j] >= 65)
	{
	  maze[j] = maze[j] - 55;
	}
      else if(maze[j] >= 48)
	{
	  maze[j] = maze[j] - 48;
	}
    }
  //eliminates the special case of the start and end cells.
  maze[0] = maze[0] + 4; // adds wall to left of starting cell
  maze[size-1] = maze[size-1] + 1; // adds wall to right of exit cell
  graph mazeGraph(dim);
  mazeGraph.BFS(maze, 0);
  mazeGraph.pathFinder();
  return 0;
}
