/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 6 CS21
*/

#ifndef MAZE_H
#define MAZE_H

#include "disjointSet.h"

using namespace std;

class maze{
 public:
  maze(int); //constructs a maze, sets variables, and creates a disjoint set.
  ~maze(); //destructor deletes all pointers.
  void mazeGenerate(); //generates a maze.
  void printMaze(); //prints the maze in matrix form.
 private:
  //static char lookUpArray[16];
  void randomizeArray(); //randomizes the randOrderArray.   
  void breakRight(int); //breaks down right wall and neighbors left wall.
  void breakLeft(int); //breaks down left wall and neighbors right wall.
  void breakTop(int); //breaks down top wall and neighbors bottom wall.
  void breakBottom(int); //breaks down bottom wall and neighbors top wall.
  int *mazePtr; //points to elements in the array of maze cell data.
  disjointSet * disSetPtr; //points to the disjointSet object.
  int *randOrderArray; //points to the randomized array of all maze indexes.
  int size; //overall number of cells in the maze.
  int dim; //dimension of the maze.
};
#endif
