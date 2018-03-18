#include "structs.h"

underline::underline(int id, int pos, int size)
{
  this->id = id;
  this->pos = pos;
  this->size = size;
  this->next = NULL;
}

underline::~underline()
{

}

int underline::get_id()
{
  return this->id;
}

int underline::get_pos()
{
  return this->pos;
}

int underline::get_size()
{
  return this->size;
}

underline* underline::get_next()
{
  return this->next;
}

underline* underline::insert(int id, int pos, int size)
{
  underline* temp = this;

  if(temp == NULL){
    temp = new underline(id,pos,size);
    return temp;
  }
  else{
    while(temp->next != NULL)
      temp = temp->next;
    temp->next = new underline(id,pos,size);
  }

  return this;
}

int underline::check_in(int pos)
{
  underline* temp = this;
  while(temp != NULL){
    if(pos == temp->pos || ((pos < temp->size + temp->pos) && (pos > temp->pos)))
      return 1;
    temp = temp->next;
  }
  return 0;
}

void underline::deleteu()
{
  underline* temp = this;
  if(temp != NULL){
    temp->next->deleteu();
    delete temp;
    temp = NULL;
  }

}
