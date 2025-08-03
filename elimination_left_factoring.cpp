#include <iostream>
#include <vector>
#include <utility>

// I took reference from https://www.tutorialspoint.com/automata_theory/left_recursion_and_left_factoring.htm
// and the examples I took in the input below are from https://www.youtube.com/watch?v=whe29gOb8p4 and also
// from https://www.youtube.com/watch?v=pfnP05Sd4FY&t=413s

// variable to hold the gamma production
std::string gamma;

// this helper function takes the productions and splits them by the delimiter '|' and then 
// pushes them into a vector and also calculates the smallest production rule
std::pair<std::vector<std::string>, int> find_tokens(std::string production)
{
  int pos = 0;
  int min_ = 1000000; // initialise min to a large number first
  std::string token;
  std::vector<std::string> tokens; // vector to hold the splitted productions

  while ((pos = production.find("|")) != std::string::npos) // while there is no more delimiter found in the sentence
  {
    if (pos < min_) // if position of delimiter < smallest length of production then smallest length of production = position
    {
      min_ = pos;
    }
    token = production.substr(0, pos); // find the first position where the delimiter occurs and substring it
    tokens.push_back(token); // push the substring into the vector
    production.erase(0, pos + 1); // delete that substring from the main string so that same delimiter doesn't get found again
  }
  gamma = production; // last production will always be gamma  
  return { tokens, min_ }; // return a pair of tokens vector and length of smallest production
}

// this function finds the alpha (smallest common prefix of the productions) and
// returns its length and also the tokens vector which it found using the above
// helper function
std::pair<int, std::vector<std::string>> find_alpha(std::string production)
{
  std::pair<std::vector<std::string>, int> result = find_tokens(production);
  std::vector<std::string> tokens = result.first;

  int min_ = result.second;
  int words = tokens.size();
  int alpha = 0;

  // this just keeps iteratively checking if the first characters of each
  // production rule are same, if they are then increment alpha and then
  // check again for the next indices of all the words
  for (int i = 0; i < min_; i++)
  {
    for (int j = 0; j < words - 1; j++)
    {
      if (!(tokens[j][i] == tokens[j + 1][i])) // if they are not same return alpha and tokens
      {
        return { alpha, tokens };
      }
    }
    alpha++; // if they are the same then you increment alpha
  }
  return { alpha, tokens };
}

// this is the main function which calls all other helper functions
void left_factoring(std::string production)
{
  std::cout << "-=- Removal of left factoring -=- ( " << production << " )"  << std::endl;
  std::string LHS = std::string(1, production[0]); // extract the NT on the LHS side and convert it into a str from char
  std::string LHS_dash = LHS + "'"; 
  std::string RHS = production.substr(3, production.length()); // extract the RHS since it starts from index 3  A->RHS
                                                               //                                               0123

  std::pair<int, std::vector<std::string>> res = find_alpha(RHS); // find alpha
  int alpha = res.first;
  std::vector<std::string> tokens = res.second;

  std::vector<std::string> beta;

  // this loop finds the beta's of all the tokens and it does that by substringing everything
  // from alpha -> token_end since thats the start and end of beta in every production rule
  for (int i = 0; i < tokens.size(); i++)
  {
    beta.push_back(tokens[i].substr(alpha, tokens[i].length() + 1));
  }
  // this line prints the LHS -> alphaLHS_dash|gamma part of the removal of left factoring.
  std::cout << LHS << "->" << tokens[0].substr(0, alpha) << LHS_dash << "|" << gamma << std::endl;
  
  // this line derives LHS_dash
  std::cout << LHS_dash << "->";

  // print out all the beta's
  for (int i = 0; i < beta.size(); i++)
  {
    if (beta[i].length() == 0) // if there is no beta, i.e the production rule is alpha then the corresponding beta will be epsilon
    {
      beta[i] = "epsilon"; 
    }
    if (i == beta.size() - 1) // this just makes it so that the extra '|' is not printed in the end 
    {
      std::cout << beta[i] << std::endl;
      break;
    }
    std::cout << beta[i] << "|";
  }
  std::cout << std::endl;
}

int main()
{
  // left_factoring("abcwaa|abcoijroa|abcwhij");
  left_factoring("A->aAB|aA|a");
  left_factoring("S->ietSeS|ietS|a");
  left_factoring("S->T+S|T|id");
  return 0;
}
