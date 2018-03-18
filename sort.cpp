#include "structs.h"
#include <iostream>

using namespace std;

void swap(query* a, query* b)
{
  query t;
  t.set_score(a->get_score());
  t.set_id(a->get_id());
  t.set_ul(a->get_ul());

  a->set_score(b->get_score());
  a->set_id(b->get_id());
  a->set_ul(b->get_ul());

  b->set_score(t.get_score());
  b->set_id(t.get_id());
  b->set_ul(t.get_ul());
}

int partition(query* score_array, int low, int high)
{
  query pivot = score_array[high];    // pivot
  int i = low - 1;  // Index of smaller element

  for(int j = low; j < high; j++){
    if (score_array[j].get_score() >= pivot.get_score()){
      i++;    // increment index of smaller element
      swap(score_array[i], score_array[j]);
    }
  }
  swap(score_array[i + 1], score_array[high]);

  return i + 1;
}

void quickSort(query* score_array, int low, int high)
{
  if(low < high){
    int pi = partition(score_array, low, high);

    quickSort(score_array, low, pi - 1);
    quickSort(score_array, pi + 1, high);
    }
}
