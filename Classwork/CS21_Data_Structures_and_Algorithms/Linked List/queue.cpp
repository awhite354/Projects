/*                                                                                         
Anthony White                                                                              
awhite@pengo.cabrillo.edu                                                                  
anthywhite@gmail.com                                                                       
Assignment 1 CS21                                                                          
*/


#include <iostream>
#include <string>

#include "queue.h"
#include "list.h"
#include "linknode.h"

using namespace std;

queue::queue(){
  //listptr = new list();
  //queue ();
  listptr = new list();
}

void queue::enqueue(string data)
{
  listptr -> insertRear(data);
}

string queue::dequeue()
{
  string result = listptr -> removeFront();
  //cout << "test";
  return result;
}

bool queue::isEmpty()
{
  if(listptr -> front == 0 || listptr -> rear == 0)
    {
      return true;
    }
  return false;
}
