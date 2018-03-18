#include <iostream>

extern int size_of_buffer;
extern float avgdl;

// CLASS USED FOR UNDERLINING

class underline {
  int id;   // document id
  int pos;  // position in article
  int size; // size of word
  underline* next;

public:
  underline(int,int,int);
  ~underline();
  int get_id();
  int get_pos();
  int get_size();
  underline* get_next();
  underline* insert(int, int, int);
  int check_in(int);
  void deleteu();
};

// CLASS USED FOR MAPPING:  id ---> article
class map {
  int id;
  char* article;
  int sz; // size of string article
  int num_of_words; // number of words in an article

public:
  map();
  ~map();
  void set_size(int);
  int get_size();
  void map_article(int);
  char* get_article();
  int get_id();
  void set_id(int);
  int get_words();
  void set_words(int);
};

// CLASS USED FOR THE END-OF-WORD LIST
class list {
  int total;      // shows how many nodes does this posting list have. It exists only in head node
  int id;
  int counter;
  list* next;

public:
  list(int);
  ~list();
  void add_total();
  int get_total();
  void add_counter();
  int get_counter();
  int get_id();
  void set_next(list*);
  list* get_next();
  void delete_list();
};

// CLASS USED FOR THE INVERSED INDEX ---> TRIE
class trie {
  char letter; // contains letter
  trie* next; // next dimension letter
  trie* dim; // same dimension letter
  bool isEnd; // mark ending of word
  list* array; // [,]
  underline* ul;  // underlining information

public:
  trie();
  ~trie();
  char get_letter();
  void set_letter(char);
  list* get_array();
  trie* insert(char*, int, int);
  void traverse();
  void traverse(char* buffer);
  void search(char*,int);
  void search_query(char**,int,int,map*,int);
  void delete_traverse();
};

// CLASS USED FOR THE SEARCH QUERIES
class query {
  int id;
  float score;
  underline* ul;

public:
  query();
  ~query();
  int get_id();
  void set_id(int);
  float get_score();
  void set_score(float);
  underline* get_ul();
  void set_ul(underline*);
};

// QUICKSORT FUNCTIONS
void swap(query*, query*);
int partition(query*, int, int);
void quickSort(query*, int, int);

// HELPING FUNCTIONS

char** get_queries(char*, int);    // minisearch.cpp
int get_counter_of_q(char*);            // minisearch.cpp
