#include "structs.h"
using namespace std;

map::map()
{
  this->num_of_words = 0;
  this->article = NULL;
}

map::~map()
{

}

void map::set_size(int sz)
{
  this->sz = sz;
}

int map::get_size()
{
  return this->sz;
}

void map::map_article(int bytes)
{
  this->article = new char[bytes];
}

char* map::get_article()
{
  return this->article;
}

int map::get_id()
{
  return this->id;
}

void map::set_id(int id)
{
  this->id = id;
}

int map::get_words()
{
  return this->num_of_words;
}

void map::set_words(int num_of_words)
{
  this->num_of_words = num_of_words;
}
