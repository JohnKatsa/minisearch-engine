#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "structs.h"

using namespace std;

int size_of_buffer; //size used for buffer in traverse
float avgdl;  // average word counter of every document

int main(int args, char* argv[])
{
  size_of_buffer = 0;
//////////////////////////////////////////////////////////////////////
  int i = 1;
  char* filename;
  int k;
  bool flagk = 0, flagn = 0;  // flag k, flag name

  for(i = 1; i < args; i++){
    if(strcmp(argv[i],"-k") == 0){
      if(argv[i+1] != NULL) k = atoi(argv[i+1]);
      flagk = 1;
    }
    if(strcmp(argv[i],"-i") == 0){
      if(argv[i+1] != NULL) filename = argv[i+1];
      flagn = 1;
    }
  }
  if(flagk == 0) k = 5;
  if(flagn == 0){ cout << "No filename given" << endl; return -1; }

  //HELPING//cout << "filename = " << filename << endl;
  //HELPING//cout << "k = " << k << endl;
//////////////////////////////////////////////////////////////////////

  char c;
  ifstream myfile(filename);

  if(myfile.is_open()){

    // COUNT THE NUMBER OF LINES IN THE DOCUMENT
    ////////////////////////////////////
    int num_of_lines = 0;
    myfile.get(c);
    while(!myfile.eof()){
      if(c == '\n') num_of_lines++;
      myfile.get(c);
    }
    //HELPING//cout << "number of lines = " << num_of_lines << endl;
    ////////////////////////////////////

    myfile.clear();             // go again at the
    myfile.seekg(0, ios::beg);  // beginning of file

    map* array = new map[num_of_lines]; // allocate space for the map array

    for(int i = 0; i < num_of_lines; i++){
      do{                   // loop
        myfile.get(c);      // until first

        // case of blank line --> error
        if(c == '\n'){
          cout << "Incorrect document!" << endl;
          delete[] array; // only array allocated till now
          return -1;
        }

      }while(c != ' ' && c != '\t');     // space

      do{                   // in case
        myfile.get(c);      // of tab

        // case of blank line --> error
        if(c == '\n'){
          cout << "Incorrect document!" << endl;
          delete[] array;   // only array allocated till now
          return -1;
        }

      }while(c == ' ' || c == '\t');     //

      // GET FROM EVERY ARTICLE ITS SIZE AND ITS WORD COUNTER
      //////////////////////////////////////////////
      array[i].set_size(0);
      array[i].set_words(1);
      int temp, tempw;
      while(c != '\n'){
        //count bytes
        temp = array[i].get_size(); temp++;
        array[i].set_size(temp);

        //count words
        if(c == ' ' || c == '\t'){
          tempw = array[i].get_words(); tempw++;
          array[i].set_words(tempw);
        }

        myfile.get(c);
      }

      array[i].map_article(array[i].get_size());  // after the size is known, allocate article space
      //////////////////////////////////////////////
    }

    // set avgdl
    float sum = 0;
    for(int i = 0; i < num_of_lines; i++){
      sum += array[i].get_words();
    }
    avgdl = sum/num_of_lines;
    //////

    myfile.clear();             // go again at the
    myfile.seekg(0, ios::beg);  // beginning of file

    // SAVE EVERY LINE IN MAP
    int check = 0;  // variable used for check of id
    for(int i = 0; i < num_of_lines; i++){
      int id;
      myfile >> id; // read id

      // checking
      if(id != check){
        cout << "Incorrect document!" << endl;
        delete[] array;    // only array allocated till now
        return -1;
      }
      check++;

      array[i].set_id(id);
      while(c != ' ' && c != '\t'){
        myfile.get(c);
      }
      while(c == ' ' || c == '\t'){
        myfile.get(c);
      }

      myfile.unget();
      myfile.getline(array[i].get_article(),array[i].get_size()+1); // +1 to also put null
      array[i].set_size(array[i].get_size()+1); // set it
    }

    myfile.clear();             // go again at the
    myfile.seekg(0, ios::beg);  // beginning of file

    //TIME TO INSERT IN THE TRIE STRUCT! INSERT BY TOKENS!
    trie* root = new trie; // root of the trie
    char* token = NULL;
    char* temp;  // temp used because token smashes original string
    for(int i = 0; i < num_of_lines; i++){
      int pos = 0;

      temp = new char[array[i].get_size()];                     // move article to temp
      memcpy(temp,array[i].get_article(),array[i].get_size());  //
      char* start = temp;

      token = strtok(temp," \t");  //token keeps every word
      while(token != NULL){
        pos = token-start;
        root = root->insert(token,array[i].get_id(),pos);

        token = strtok(NULL," \t");
      }
      delete[] temp;  // free buffer
    }

    //SELECTION
    char* buf = new char[200];
    int size;
    while(1){

      //print of options
      cout << "These are the functions supported: " << endl;
      cout << "Function 1 : search. Type search [word_to_search (max 10 words)] -----> Prints nearest article to given words." << endl;
      cout << "Function 2 : df. Type df [word_to_print_frequency] or type df.   -----> Prints frequency of given word, or of all if any word given." << endl;
      cout << "Function 3 : tf. Type tf [document_id] [word]                    -----> Prints frequency of given word to given document." << endl;
      cout << endl << "Type exit to quit." << endl << endl;

      cout << "/";
      size = 0;
      buf[0] = getchar();
      while(size != 200 && buf[size] != '\n'){
        //cout << buf << endl;
        size++;
        buf[size] = getchar();
      }

      cout << "-------------------------------------------------------------------------------" << endl;

      // EXIT
      if(strncmp(buf,"exit",4) == 0){

        // TIME TO FREE MY STRUCTS!
        root->delete_traverse();    // delete trie
        delete[] buf;               // delete input command buffer
        delete[] array;             // delete mapping

        break;
      }
      // SEARCH
      else if(strncmp(buf,"search",6) == 0){
        int counter_of_q = get_counter_of_q(buf);

        char** words = get_queries(buf, counter_of_q);

        root->search_query(words,counter_of_q,num_of_lines,array,k);

      }
      // DF
      else if(strncmp(buf,"df",2) == 0){
        if(buf[2] != '\n'){ // exists given word
          char** words = get_queries(buf, 1);     // df is only for 1 word
          root->search(words[0], -1);

          delete[] words;   // delete args
        }
        else{   // no specific word given, print all
          char buffer[256];
          root->traverse(buffer);
        }
      }
      // TF
      else if(strncmp(buf,"tf",2) == 0){
        if(buf[2] == '\n')    // error no argument given
          continue;

        char** words = get_queries(buf, 2);   // tf needs one id and one word
        root->search(words[1],atoi(words[0]));

      }

      for(int j = 0; j < 200; j++){
        buf[j] = '\0';
      }
      cout << "-------------------------------------------------------------------------------" << endl;
    }

    myfile.close();
  }

  return 0;
}

int get_counter_of_q(char* buf)
{
  // find counter of q
  int offset = 0;
  int counter_of_q = 0;
  while(buf[offset] != '\n'){
    if(buf[offset] == ' '){
      counter_of_q++;
    }
    offset++;
  }

  // if given more than 10 words (used in search,) take only first 10
  if(counter_of_q <= 10)
    return counter_of_q;

  return 10;
}

char** get_queries(char* buf2, int counter_of_q)   // doesnt make new space... it manipulates buf
{

  char** words = new char*[counter_of_q];
  char* buf = new char[200];
  memcpy(buf,buf2,200);

  int i = 0;
  char* token = strtok(buf," \t");  //token = search... we dont want it

  words[i] = strtok(NULL, " \t\n");

  while(words[i] != NULL){
    i++;
    words[i] = strtok(NULL," \t\n");
  }

  return words;
}
