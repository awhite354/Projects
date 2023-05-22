/*
Anthony White
awhite@pengo.cabrillo.edu
anthywhite@gmail.com
Assignment 8 CS21
*/

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>

#include "graph.h"

using namespace std;

graph::graph(int n){
  dim = n;
  size = n*n;
  for(int i = 0 ; i < size; i++)
    {
      d.push_back(0);
      color.push_back("white");
      p.push_back(0);
    }
}

void graph::BFS(vector<int> G, int s){
  queue <int> Q;
  for(int i = 0; i < size; i++)
    {
      color[i] = "white";
      d[i] = INT_MAX;
      p[i] = -1;
    }
  color[s] = "gray";
  d[s] = 0;
  p[s] = -1;
  Q.push(s);
  while(!Q.empty())
  {
      int t = Q.front();
      Q.pop();
      vector <int> adjacency = findAdjacent(G[t], t);
      for(int j = 0; j < adjacency.size(); j++)
	{
	  if(color[adjacency[j]] == "white")
	    {
	      color[adjacency[j]] = "gray";
	      d[adjacency[j]] = d[t] + 1;
	      p[adjacency[j]] = t;
	      Q.push(adjacency[j]);
	    }
	}
      color[t] = "black";
  }
  return;   
}

vector<int> graph::findAdjacent(int hexVal, int vertex){
  vector<int> V;  
  if((hexVal & 1) == 0)
    {
      V.push_back(vertex + 1);
    }
  if((hexVal & 2) == 0)
    {
      V.push_back(vertex + dim);
    }
  if((hexVal & 4) == 0)
    {
      V.push_back(vertex - 1);
    }
  if((hexVal & 8) == 0)
    {
      V.push_back(vertex - dim);
    }
  return V;
}

void graph::pathFinder(){
  int cell  = (size - 1);
  int x, y;
  vector <int> pathTrace;
  do
    { 
      pathTrace.push_back(cell);
      cell = p[cell];
    }
  while(cell != -1);

  reverse(pathTrace.begin(), pathTrace.end());
  
  for(int i = 0; i < pathTrace.size(); i++)
    {
      x = (pathTrace[i] % dim);
      y = (pathTrace[i] / dim);
      cout << "(" << x << ", " << y << ")" << endl;
    }
}
