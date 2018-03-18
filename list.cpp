#include "structs.h"
using namespace std;

list::list(int id)
{
  this->total = 1;
  this->id = id;
  this->counter = 1;
  this->next = NULL;
}

list::~list()
{

}

void list::add_total()
{
  this->total++;
}

int list::get_total()
{
  return this->total;
}

void list::add_counter()
{
  this->counter++;
}

int list::get_counter()
{
  return this->counter;
}

int list::get_id()
{
  return this->id;
}

void list::set_next(list* l)
{
  this->next = l;
}

list* list::get_next()
{
  return this->next;
}

void list::delete_list()
{
  list* temp = this;
  if(temp != NULL){
    temp->next->delete_list();
    delete temp;
    temp = NULL;
  }
}
