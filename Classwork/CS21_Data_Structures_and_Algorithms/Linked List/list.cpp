/*                                                                                         
Anthony White                                                                              
awhite@pengo.cabrillo.edu                                                                  
anthywhite@gmail.com                                                                       
Assignment 1 CS21                                                                          
*/



#include <iostream>
#include <string>

#include "list.h"

using namespace std;

list::list()
{
  front = 0;
  rear = 0;
}

list::~list()
{
  linknode *temp = front;
  while(front)
    {
      temp = front;
      front = front -> next;
      delete temp; temp = 0;
    }
}

void list::insertFront(string data)
{
  linknode *temp = new linknode();
  temp -> data = data;
  if(!front)
    {
      front = rear = temp;
      return;
    }
  front -> prev = temp;
  temp -> next = front;
  front = temp;
}

void list::insertRear(string data)
{
  linknode *temp = new linknode();
  temp -> data = data;
  if(!front)
    {
      front = rear = temp;
      return;
    }
  rear -> next = temp;
  temp -> prev = rear;
  rear = temp;
}

string list::removeFront()
{
  if(!front)
    {
      //list is empty
      string result = "/n";
      return result;
    }
  string result = front -> data;
  linknode *temp = front;
  if(front == rear)
    {
      delete temp; temp - 0;
      front = rear = 0;
      return result;
    }
  temp -> next -> prev = 0;
  front = front -> next;
  delete temp; temp = 0;
  return result;
}

string list::removeRear()
{
  if(!rear)
    {
      //list is empty
      string result = "/n";
      return result;
    }
  string result = rear -> data;
  linknode *temp = rear;
  if(rear == front)
    {
      delete temp; temp = 0;
      rear = front = 0;
      return result;
    }
  temp -> prev -> next = 0;
  rear = rear -> prev;
  delete temp; temp = 0;
  return result;
}
