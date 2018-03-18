#include <iostream>
#include "structs.h"

using namespace std;

query::query()
{
  this->ul = NULL;
}

query::~query()
{

}

int query::get_id()
{
  return this->id;
}

void query::set_id(int id)
{
  this->id = id;
}

float query::get_score()
{
  return this->score;
}

void query::set_score(float score)
{
  this->score = score;
}

underline* query::get_ul()
{
  return this->ul;
}

void query::set_ul(underline* ul)
{
  this->ul = ul;
}
