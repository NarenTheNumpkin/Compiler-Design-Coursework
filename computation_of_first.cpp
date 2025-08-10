#include <iostream>
#include <vector>
#include <map>
#include <cctype>
// NOTE: I took reference from https://www.tutorialspoint.com/what-are-first-and-follow-and-how-they-are-computed

/* Computation of FIRST(X)
 *    
 *    - If X is a terminal symbol, then FIRST(X) = {X}
 *    
 *    - If X -> ε, then FIRST(X) = {ε}
 *
 *    - If X is non-terminal and X -> a⍺, then FIRST(X) = {a}
 *
 *    - If X -> Y1Y2Y3, then FIRST(X) will be 
 *          
 *        a) If Y1 is terminal then FIRST(X) = {Y1}
 *        b) If Y1 is non-terminal and FIRST(Y1) DOES NOT contain/derive ε, then FIRST(X) = FIRST(Y1)
 *        c) If Y1 is non-terminal and FIRST(Y1) DOES contain/derive ε, then FIRST(X) = FIRST(Y1) - ε ⋃ FIRST(Y2, Y3)
 */

// These maps store the NT->index and index->NT mapping, i.e if you give me non terminal I
// will tell you where its production index is in the prods and other vectors and vice versa.
// this is just to make my life easier (don't worry about it)
std::map<std::string, int> NT_to_index;
std::map<int, std::string> index_to_NT;

// dummy function to print values in a vector so I don't have to always type it again and again
void print(std::vector<std::string> inp)
{
  for (int i = 0; i < inp.size(); i++)
  {
    std::cout << inp[i] << std::endl;
  }
}

// function which initialises the above maps
void map_maker(std::vector<std::string> productions)
{
  for (int i = 0; i < productions.size(); i++)
  {
   NT_to_index[std::string(1, productions[i][0])] = i; 
   index_to_NT[i] = std::string(1, productions[i][0]);
  }
}

// This parses the productions based on the delimiter, i.e if production is in E->+ET|ak
// then it would parse it into a vector like [+ET, ak] for the function call parse_productions(E->+ET|ak, "|")
std::vector<std::string> parse_productions(std::string raw_productions, std::string delimiter)
{
  std::string production; 
  std::vector<std::string> productions;
  int pos = 0;

  // this finds the first occurance of the delmiter and then substrings it and adds it to the vector
  // and then it removes that substring from the main string so the same index of the delmiter
  // is not found again, it does this till no delimiter is found.
  while ((pos = raw_productions.find(delimiter)) != std::string::npos)
  {
    production = raw_productions.substr(0, pos);
    productions.push_back(production);
    raw_productions.erase(0, pos + 1);
  }
  productions.push_back(raw_productions);

  return productions;
}

std::vector<std::vector<std::string>> find_prods(std::vector<std::string> productions)
{
  std::vector<std::vector<std::string>> prods;
   for (int i = 0; i < productions.size(); i++)
 {
   std::vector<std::string> res = parse_productions(productions[i], "|");
   res[0].erase(0, 3); // remove the "NT ->" part from the producion so everything is uniform and nice
   prods.push_back(res);
 }
  return prods;
}

// helper function which contains main logic for computing FIRST()
std::vector<std::string> _FIRST(std::vector<std::vector<std::string>> prods, int index)
{
  std::vector<std::string> first;
  std::vector<std::string> prod = prods[index];
  for (int i = 0; i < prod.size(); i++)
  {
    if ( (isalpha(prod[i][0]) && std::islower(prod[i][0])) || (!isalpha(prod[i][0])) ) // if terminal or not a non terminal
    {
      first.push_back(std::string(1, prod[i][0]));
    }
    else if (!std::islower(prod[i][0])) // If non terminal
    {
      // add the FIRST(Y1) to FIRST(X)
      std::vector<std::string> _first = _FIRST(prods, NT_to_index[std::string(1, prod[i][0])]); 
      first.insert(first.end(), _first.begin(), _first.end());
    }
  }
  return first;
}

// Main function which computes the FIRST() for every non terminal
std::vector<std::string> FIRST(std::string inp)
{
  std::vector<std::string> productions = parse_productions(inp, " "); // Split the initial input into distinct productions seperated by spaces
  std::vector<std::string> firsts; // vector to store the terminals of FIRST()
  std::vector<std::vector<std::string>> prods; // this vector is to hold the multiple productions for a single non terminal

  map_maker(productions); // This creates the NT->index and index->NT maps so I can easily retrive the values
  prods = find_prods(productions); // this calls find_prods() which splits a non terminal which has multiple productions based on '|' delimiter
  
  std::vector<std::string> res; // to hold the result of the computation of FIRST()

  for (int i = 0; i < prods.size(); i++)
  {
   res = _FIRST(prods, i); // _FIRST() is a helper function which actually consists of the implementation logic
   std::cout << "-=- FIRST(" << index_to_NT[i] << ") -=-" << std::endl;
   print(res);
  }
  return firsts;
}


int main() 
{
  // std::string productions = "S->(L)|a L->SK K->,SK|e";
  std::string productions = "E->TK K->+TK|e T->FZ Z->*FZ|e F->(E)|i";
  FIRST(productions);

  return 0;
}
