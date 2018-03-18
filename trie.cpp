#include "structs.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <sys/ioctl.h>  // used for
#include <unistd.h>     // ioctl

#include <stdio.h>  // print at end of search_query

using namespace std;

trie::trie()
{
  this->letter = '\0';  // my identifier
  this->next = NULL;
  this->dim = NULL;
  this->isEnd = 0;
  this->array = NULL; // dont know if end of word
  this->ul = NULL;
}

trie::~trie()
{

}

char trie::get_letter()
{
  return this->letter;
}

void trie::set_letter(char letter)
{
  this->letter = letter;
}

trie* trie::insert(char* word, int id, int pos)
{
  int size = strlen(word);              //
  //cout << "strlen = " << size << endl;  //
  //cout << pos << endl;                  //

  char temp = word[0];
  trie* root = this;
  trie* p = this;   // initially point at root
  trie* previous = p; // used for next |
  trie* prev; // used for dim --

  while(temp != '\0'){    // for every letter

    // INITIALLY BLANK
    if(p->letter == '\0'){
      p->letter = temp;
      word++;
      temp = word[0];
      if(p->next == NULL){
        p->next = new trie;
      }
      previous = p;
      p = p->next;
      continue;
    }

    // MANY DIMENSIONAL NODES
    prev = p;
    int flag = 0;
    while(p->dim != NULL){
      if(p->letter == temp){
        break;
      }
      else if(p->letter < temp){
        prev = p;
        p = p->dim;
      }
      else{
        //previous;
        if(prev != p){
          prev->dim = new trie;         // create new dim
          prev->dim->letter = temp;     // set letter
          prev->dim->dim = p;
          prev->dim->next = new trie;   // create null next
          p = prev->dim;
          flag = 1;
          break;
        }
        else{
          prev = new trie;
          prev->letter = temp;
          prev->dim = p;
          prev->next = new trie;
          p = prev;

          if(p->dim == root) // if(root == previous)?
            root = p;    // change root to new min
          else
            previous->next = prev;  // point to new min if not root!

          flag = 1;
        }
      }
    }


    // not in because of 1 node dimension, check 3 cases
    if(p->letter == temp){
      //p;
    }
    else if(p->letter < temp){
      p->dim = new trie;
      p->dim->letter = temp;
      p = p->dim;
      p->next = new trie;
    }
    else if(p->letter > temp && flag == 0){
      //previous;
      trie* node = new trie;
      node->dim = p;
      node->letter = temp;
      if(previous->next == p) previous->next = node;
      if(prev != p) {prev->dim = node; node->dim = p;}  // in case it comes from multidimensional case
      if(node->next == NULL)
        node->next = new trie;  // make next null node

      //previous->next = node;
      if(root == p){
        root = node;
      }

      word++;
      temp = word[0];
      if(previous != p)
        previous = previous->next; // john-ny john-athan
      else
        previous = node;

      p = node->next;     //
      continue;
    }

    word++;
    temp = word[0];
    if(p->next == NULL){
      p->next = new trie;
    }
    if(temp != '\0'){
      previous = p;
      p = p->next;
    }
  }

  // ENDING OF WORD. MAKE THE ARRAY OF FREQUENCIES, OR JUST ADD
  if(p->letter == '\0'){
    p = previous;
  }

  p->ul = p->ul->insert(id,pos,size);    // insert underline information

  p->isEnd = 1;

  if(p->array == NULL){ // WORD FOUND FOR FIRST TIME
    p->array = new list(id);
  }
  else{ // WORD FOUND IN OTHER IDS
    list* tmp = p->array;

    while(tmp != NULL){
      if(tmp->get_id() == id){ // id found, already existing
        tmp->add_counter();
        break;
      }
      tmp = tmp->get_next();
    }
    if(tmp == NULL){ // TRAVERSED WHOLE LIST AND FOUND NOTHING
      list* tmp = p->array;
      tmp->add_total(); // new node ---> need to inform head node
      while(tmp->get_next() != NULL){
        tmp = tmp->get_next();
      }

      list* tmp2 = new list(id);
      tmp->set_next(tmp2);
    }
  }
  return root;

}

list* trie::get_array()
{
  return this->array;
}

void trie::search(char* word, int id)
{
  trie* root = this;
  trie* previous = this;
  trie* last = this;		// used to know if word is real and not just a subword
  int offset = 0;
  while(word[offset] != '\0'){
    while(root != NULL){
      if(root->letter == word[offset]){
        break;
      }
      root = root->dim;
    }
    last = root;
    if(root == NULL){
      break;
    }
    offset++;
    previous = root;
    root = root->next;
  }

  if(last == NULL)	return;

  list* l = previous->get_array();
  if(l == NULL) return;

  if(id == -1){    // df call
    cout << word << " " << l->get_total() << endl;
  }
  else{
    while(l != NULL){
      if(l->get_id() == id){
        cout << id << " " << word << " " << l->get_counter() << endl;
        return;
      }
       l = l->get_next();
    }
  }

}

char* add_buffer(char* buffer, trie* node)
{
  buffer[size_of_buffer] = node->get_letter();
  size_of_buffer++;
  return buffer;
}

char* sub_buffer(char* buffer)
{
  buffer[size_of_buffer] = '\0';
  size_of_buffer--;
  return buffer;
}

void trie::traverse(char* buffer)
{
  buffer = add_buffer(buffer,this);
  if(this->isEnd == 1){
    cout << buffer; //<< endl;
    list* l = this->array;
    int sum = 0;
    while(l != NULL){
      sum += 1;         //l->get_counter();  // 1 for article_times, counter for word_times
      l = l->get_next();
    }
    cout << " " << sum << endl;
  }
  if(this->next->letter != '\0'){
    this->next->traverse(buffer);
  }
  if(this->dim != NULL){
      buffer = sub_buffer(buffer);
      this->dim->traverse(buffer);
      return;
  }
  else{ buffer = sub_buffer(buffer); return; }
}

void trie::search_query(char** words, int counter, int num_of_lines, map* array, int k)
{
  query* score_array = new query[num_of_lines];   // score array --> for every document
  for(int i = 0; i < num_of_lines; i++){   // basically num_of_lines = number of documents
    score_array[i].set_score(0.0);    // all initially zero
    score_array[i].set_id(i);
  }

  // for every word in query
  for(int i = 0; i < counter; i++){

    // find word's posting list
    trie* root = this;
    trie* previous = this;
    trie* last = this;		// used to know if word is real and not just a subword
    int offset = 0;
    char* word = words[i];
    while(word[offset] != '\0'){
      while(root != NULL){
        if(root->letter == word[offset]){
          break;
        }
        root = root->dim;
      }
      last = root;
      if(root == NULL){
        break;
      }
      offset++;
      previous = root;
      root = root->next;
    }

    if(last == NULL)	continue;

    //underline* test = previous->ul;
    //while(test != NULL){
    //  cout << test->get_id() << " " << test->get_pos() << " " << test->get_size() << endl;
    //  test = test->get_next();
    //}

    //insert in score_array underlining information
    if(previous->ul != NULL){
      underline* u2 = previous->ul;                       // pointer to trie node ul
      underline* u = score_array[u2->get_id()].get_ul();  // pointer to score_array ul
      while(u2 != NULL){
        //cout << u2->get_id() << " " << u2->get_pos() << " " << u2->get_size() << endl;
        u = u->insert(u2->get_id(),u2->get_pos(),u2->get_size());
        score_array[u2->get_id()].set_ul(u);

        u2 = u2->get_next();                    // iterative
        if(u2 != NULL)
          u = score_array[u2->get_id()].get_ul(); // move
      }
    }

    // run posting list
    list* l = previous->get_array();
    list* linit = l;  // lprev holds list head

    while(l != NULL){
      float idf, ans, ansup, ansdown;

      idf = log10((num_of_lines - linit->get_total() + 0.5)/(linit->get_total() + 0.5));

      ansup = (float)((float)(l->get_counter())/array[l->get_id()].get_words())*(2.2);

      ansdown = ((float)(l->get_counter()/array[l->get_id()].get_words()) + 1.2*(0.25 + 0.75*((float)array[l->get_id()].get_words()/avgdl)));

      ans = ansup/ansdown;
      score_array[l->get_id()].set_score(score_array[l->get_id()].get_score() + idf*ans);

      l = l->get_next();
    }
  }

  // TEST PRINT
  /*underline* u;
  for(int i = 0; i < num_of_lines; i++){
    cout << i << ". ";
    u = score_array[i].get_ul();
    while(u != NULL){
      cout << u->get_id() << " " << u->get_pos() << " " << u->get_size() << endl;
      u = u->get_next();
    }
  }*/

  quickSort(score_array,0,num_of_lines-1);  // sort score table

  // GET WINDOW SIZE
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  // w.ws_col shows full window size <------> (width)

  // print scores
  int i = 0;
  int num = 1;      // to print the number of document in printing order
  while(i < num_of_lines && num < k+1){
    if(score_array[i].get_score() != 0.0){
      printf("%3d.(%4d)[%2.5f] ", num, score_array[i].get_id(), score_array[i].get_score());
      char* c = array[score_array[i].get_id()].get_article();

      underline* pr = score_array[i].get_ul();
      int pos = 0;

      for(int j = 21; j < w.ws_col; j++){
        if(c[0] != '\0'){
          cout << c[0];
          c++;
        }
      }
      for(int j = 0; j < 21; j++){
        cout << " ";
      }
      for(int j = 21; j < w.ws_col; j++){
        if(pr->check_in(pos)){
          cout << "\033[1;31m^\033[0m";
          //cout << "^";
        }
        else
          cout << " ";
        pos++;
      }

      int k = 0;
      int flag = 0;         // even -> article, odd -> highlight
      while(c[0] != '\0'){
        if(k == w.ws_col){
          k = 0;
          flag++;
        }
        if(k < 21){
          cout << " ";
        }
        else{
          if(flag%2 == 0){
            cout << c[0];
            c++;
          }
          else{
            if(pr->check_in(pos)){
              cout << "\033[1;31m^\033[0m";
              //cout << "^";
            }
            else
              cout << " ";
            pos++;
          }
        }
        k++;
      }
      // fill in last line if it has blanks
      while(k < w.ws_col){
        cout << " ";
        k++;
      }

      // underline last line
      for(int j = 0; j < 21; j++){
        cout << " ";
      }
      for(int j = 21; j < w.ws_col; j++){
        if(pr->check_in(pos)){
          cout << "\033[1;31m^\033[0m";
          //cout << "^";
        }
        else
          cout << " ";
        pos++;
      }

      cout << endl;
      num++;
      cout << endl;
    }
    i++;
  }

  delete[] score_array;
}

void trie::delete_traverse()
{
  if(this->next != NULL)
    this->next->delete_traverse();

  if(this->dim != NULL)
    this->dim->delete_traverse();

  if(this->array != NULL){
    this->array->delete_list();
  }

  if(this->ul != NULL){
    this->ul->deleteu();
  }

  delete this;
}
