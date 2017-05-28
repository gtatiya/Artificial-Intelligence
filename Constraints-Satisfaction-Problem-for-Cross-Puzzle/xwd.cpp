#include <iostream>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <assert.h>
#include "Grid.h"
#include "Lexicon.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;

/*
  Some useful information for interacting with the other provided
  classes:
  1) To find all the words that match a certain pattern, use 
    lexicon->find_matches(pattern, vector_of_words_to_ignore);
    For example, if we've already used the words "BEAR" and "TRAP" (as
    contained in some vector vec) and want to find all the words that
    fit the pattern GR?ZZ??, and we have lexicon lex, we would call
    lex->find_matches("GR?ZZ??",vec)
  2) To find out the identity of a word keyed by some integer, use
    lex->get_word(key). For example, if we want to know the identity of
    word 34, we would call lex->get_word(34).
  3) The crossword grid contains a list of words to fill in. You can
    access this list with grid->words.
  4) In order to actually fill in word w, you just need to do
    w->set(pattern). For example, if w previously contained "J?R", and
    you call w->set("JAR"), w will now contain "JAR", and the word
    crossing the middle of w will be updated to contain an "A".
 */


/*
  Use Me!
  =======
  An ASSIGNMENT consists of:
  1. A Word* (comprised of a series of slots)
  2. The int shorthand for the string assigned to that series of slots.
*/
struct ASSIGNMENT{
  Word* entry;
  int value;
};


/* 
   Fill me in!
   ===========
   Purpose: choose_entry should use the Minimum Remaining Values (MRV)
            heuristic to choose the crossword slot (Word*) for which
            the fewest options remain. 
   Argument: lex -- a Lexicon* (used to find words that fit a certain
             string pattern, using lex->find_matches([some string],chosen_values)) 
   Argument: entry_options -- a vector* of Word*s left to choose
   Argument: chosen_values -- a vector* of int shortands for strings
             that are not available for consideration anywhere in the
             puzzle  
   Returns: the Word* that should be filled in next
   Side effects: the chosen Word* is removed from entry_options
 */

Word* choose_entry(Lexicon* lex, vector<Word*>* entry_options, vector<int>* chosen_values){
	Word* chosen_variable;
	vector<Word*>::iterator it, entry;
	int large_no = 99999; // Just a large number

	// loop for every element in 'entry_options'
    for (it = entry_options->begin(); it != entry_options->end(); ++it)
    {
    	string pattern = (*it)->string_form();

        int choices = lex->find_matches(pattern, chosen_values)->size(); // number of choices available

        // search for the minimum number of choices available
        if(choices < large_no)
        {
            large_no = choices;
            entry = it;
            chosen_variable = *it;
        }
    }
    
    entry_options->erase(entry); // deleting slected choice from entry_options
    return chosen_variable;
}


/* 
   Fill me in!
   ===========
   Purpose: choose_value should use the Least Constraining Value (LCV)
            heuristic to choose the string to fill into the specified
            slot for which the largest number of options will remain
            in the puzzle. It is up to you what "Least Constraining"
            means (i.e., if it's computed using a sum or min
            operation) so long as you can justify your choice.
   Argument: grid -- a crossword Grid* as defined above
   Argument: lex -- a Lexicon* (used to find words that fit a certain
             string pattern, using lex->find_matches([some string],chosen_values))  
   Argument: entry -- a Word* we want to fill in
   Argument: value_options -- a vector* of int shorthands for strings
             that can be filled into entry 
   Argument: chosen_values -- a vector* of int shorthands that are not
             available for consideration anywhere in the puzzle 
   Returns: the int shorthand of the string to fill into entry.
   Side effects: the chosen integer is removed from value_options
                 Note: If you modify entry while trying different
                 options, make sure it is reset to it's initial
                 pattern before the function returns 
*/

int choose_value(Grid* grid, Lexicon* lex, Word* entry, vector<int>* value_options, vector<int>* chosen_values){
	int small = 0;
    string entry_temp = entry->string_form();
    vector<int>::iterator val;
    int chosen_value;
    srand (time(NULL)); // Initialize random number generator
    val = value_options->begin() + rand() % value_options->size(); // val points to random subset in value_options
    
        int choices = 0;
        string word = lex->get_word(*val); // identity of a word keyed by some integer (*val)
        entry->set(word); // set the word to entry
		vector<Word*>::iterator it;

		// loops for list of words to fill in
        for (it = grid->words.begin(); it != grid->words.end(); it++) {
            string pattern = (*it)->string_form();
            choices += lex->find_matches(pattern, chosen_values)->size(); // number of choices available
        }

        // search for the maximum number of choices available
        if (choices > small) {
            small = choices;

        }

    chosen_value = *val;
    value_options->erase(val); // deleting slected choice from value_options
    entry->set(entry_temp);
    return chosen_value;
}

//DO NOT MODIFY ANYTHING BELOW THIS POINT
//=============================================================================

bool solve(Lexicon* lex,
           Grid* grid,
           vector<Word*>* entry_options,
           vector<int>* chosen_values,
           deque<ASSIGNMENT>*solution){
  //1) Choose a slot to fill in
  Word* chosen_entry = choose_entry(lex, entry_options, chosen_values);
  
  //2) Find the words we could fill into that slot
  vector<int>* value_options = lex->find_matches(chosen_entry->string_form(),chosen_values);
  
  //3) Choose from among those options...
  while(!value_options->empty()){
    int chosen_value = choose_value(grid, lex, chosen_entry, value_options, chosen_values);
    //cout<<"chose: "<<lex->get_word(chosen_value)<<endl;
    ASSIGNMENT assignment;
    assignment.entry = chosen_entry;
    assignment.value = chosen_value;

    //...and add it to the solution set.
    chosen_entry->set(lex->get_word(chosen_value));
    chosen_values->push_back(chosen_value);
    solution->push_back(assignment);
    grid->printGrid();

    //If the puzzle is filled, return true; we're done!
    if(entry_options->empty()) {
      delete value_options;
      return true;
    }

    //Otherwise, recurse!
    vector<int>* chosen_clone =new vector<int>(*chosen_values);
    vector<Word*>* entry_options_clone = new vector<Word*>(*entry_options);
    bool result = solve(lex,
                        grid,
                        entry_options_clone,
                        chosen_clone,
                        solution);
    delete chosen_clone;
    delete entry_options_clone;

    //If we've reached this point through success, do cleanup and
    //return true;
    if(result) {
      delete value_options;
      return true;
    }else{
      //Otherwise (if we've reached this point through failure), roll
      //back to the previous grid.
      chosen_values->pop_back();
      solution->pop_back();
      grid->clear();
      for(unsigned int i = 0; i < solution->size(); i++)
        solution->at(i).entry->set(lex->get_word(solution->at(i).value));
    }
  }
  //If we've reached this point, it's because we're out of
  //options. Return false.
  delete value_options;
  return false;
}

string readGrid(char* gridname){
  ifstream ifs(gridname);
  string cs((istreambuf_iterator<char>(ifs)),(istreambuf_iterator<char>()));
  char chars[] = "\\\n";
  for (unsigned int i = 0; i < strlen(chars); ++i)
    cs.erase(remove(cs.begin(),cs.end(),chars[i]),cs.end());
  return cs;
}

int main(int argc, char** argv){
  const char* dictname = "nyt.dict";
  char* gridname = argv[1];
  int gridlength = atoi(argv[2]);
  string gridstring = readGrid(gridname);
  Lexicon* lex = new Lexicon(dictname, gridlength); 
  Grid* grid = new Grid(gridstring,gridlength);  
  grid->printGrid();

  deque<ASSIGNMENT>* solution = new deque<ASSIGNMENT>();
  vector<int>* chosen_values = new vector<int>();
  vector<Word*>* entry_options = new vector<Word*>(grid->words);
  bool solved = solve(lex,grid,entry_options, chosen_values, solution);
  if(solved) cout<<"Puzzle Solved!"<<endl;
  else cout<<"No Solution Found."<<endl;

  grid->printGrid();

  delete chosen_values;
  delete solution;
  delete entry_options;
  delete lex;
  delete grid;
  return 0;
}
