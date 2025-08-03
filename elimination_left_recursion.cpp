#include <iostream>
#include <vector>

// I took reference from https://www.tutorialspoint.com/automata_theory/left_recursion_and_left_factoring.htm
// all the inputs I tested below in the program are from that article 

// this is the function where the removal of left_recursion takes place
void left_recursion(std::string production)
{
  std::string A = std::string(1, production[0]); // I extract the first NT from production rule and convert it into str from char
  
  // vectors for alpha and beta
  std::vector<std::string> alpha; 
  std::vector<std::string> beta;

  /* this is global loop variable so I can keep track of where the loop ended when lookng for | and 
   * determine 'beta'
  */
  int i; 

  //                                         01234  
  // here I took 4 before in every input its Z->Zalpha and the starting of alpha always starts from the 4th index up until '|' 
  for (i = 4; i < production.length(); i++)
  {
    if (production[i] == '|') // check if '|' is found, that means alpha string is complete and then break out of loop.
    {
      break;
    }
    alpha.push_back(std::string(1, production[i])); // if '|' not found then add the symbols to the alpha vector
  } 

  // now I know where 'i' terminated (at '|') so the remaining string after '|' until the end is just beta
  for (int j = i + 1; j < production.length(); j++)
  {
    beta.push_back(std::string(1, production[j])); 
  }

  std::cout << "-=- Eliminating left recursion -=-" << std::endl;
  std::cout << A << " -> "; // A here was the first NT I extracted.

  // Here its A -> beta
  for (int i = 0; i < beta.size(); i++)
  {
    std::cout << beta[i];
  }

  // Here 'K' referes to alpha_dash since we can assign it to any dummy variable of our choice
  // and now from previous print we got A -> beta. now we get A -> betaK
  std::cout << "K" << std::endl;

  // Derive K here as well
  std::cout << "K -> ";

  // K -> alpha
  for (int i = 0; i < alpha.size(); i++)
  {
    std::cout << alpha[i];
  }
  // K -> alphaK/epsilon
  std::cout << "K/epsilon";

}

int main(int argc, char* argv[])
{
  /* 
   * I use a mac so I can't give '->' or '/' symbol as 
   * command line input (since unix systems think '/' is directory path`) 
   * thats why I just gave inputs during compile time 
  */

  // std::string arg1(argv[1]);
  // std::cout << arg1;
  left_recursion("E->E+T|T");
  std::cout << std::endl << std::endl;
  left_recursion("T->T*F|F");
  // left_recursion(arg1);
  return 0;
}
