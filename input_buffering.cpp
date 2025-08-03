#include <iostream>
#include <vector>
#include <map>
#include <cctype>

// Create buffer size 
#define BUFFER_SIZE 16 

// SymbolTable Map which maps each token to symbol 
std::map<std::string, std::string> SymbolTable = {
	{"switch", "keyword"},
	{"long", "keyword"},
	{"for", "keyword"},
	{"else", "keyword"},
	{"continue", "keyword"},
	{"break", "keyword"},
	{"struct", "keyword"},
	{"short", "keyword"},
	{"float", "keyword"},
	{"double", "keyword"},
	{"const", "keyword"},
	{"void", "keyword"},
	{"case", "keyword"},
	{"default", "keyword"},
	{"typedef", "keyword"},
	{"if", "keyword"},
	{"int", "keyword"},
	{"return", "keyword"},
	{"while", "keyword"},
	
	{"+", "operator"},
	{"-", "operator"},
	{"*", "operator"},
	{"/", "operator"},
	{"=", "operator"},
	{"%", "operator"},
	{">", "operator"},
	{"<", "operator"},
	{"!", "operator"},
  {",", "operator"}
};

// to check if the keyword is a operator
bool isOperator(char character) 
{
    std::string s(1, character);
    /* This is boolean checking of the condition like if the token exists 
     * in symbol table and is also an operator
     *
     * ex: 1 0 -> token exists in symbol table but it isn't an operator
     *     1 1 -> token exist and also is an operator
     *
     *     0 1 case is impossible becauase it cannot not exist in symbol 
     *         table and still have a value
     */
    return SymbolTable.find(s) != SymbolTable.end() && SymbolTable[s] == "operator";
}

// to check if the token is a keyword
bool isKeyword(std::string token)
{
  // Same checking logic as isOperator() function
	return SymbolTable.find(token) != SymbolTable.end() && SymbolTable[token] == "keyword";
}

void inputBuffer(std::string input)
{
	auto start_it = input.begin(); // start iterator pointing to the beginnig of the string 
	auto end_it = input.begin() + BUFFER_SIZE; // end iterator pointing to the end of the input
	std::vector<char> buffer(start_it, end_it); // copy all the content b/w start_it and end_it into the vector buffer
	buffer.push_back('\0'); // this is sentinel symbol I added so I can recognise when the buffer is full
	
	char* bptr = &buffer[0]; // begin pointer pointing at start of buffer
	char* fptr = &buffer[0]; // forward pointer also initially pointing to the start

	while (*fptr != '\0') // while buffer is not full (or) till sentinel is not encountered
	{
		if (fptr == &buffer.back())
		{
			std::cout << "Reached end of buffer, overwriting buffer\n";
			break;
		}
		fptr++; // keep incrementing forward pointer until we don't reach the sentinel

    // if we see empty space then we check content b/w fptr and bptr and index it into the SymbolTable
    // to see if the corresponding token is (operator) or (keyword)
		if (*fptr == ' ' || *fptr == ',') 
		{
      // if we fptr reaches a space or a ',' then I make a string from the content between
      // the bptr and fptr and index their content to the symbol table to check if a valid
      // token exsits otherwise its an identifier
			std::string token(bptr, fptr);
			if (isKeyword(token))
			{
				std::cout << "Token detected (keyword) from address " << (void*)fptr << " " << (void*)bptr << std::endl;
			}
      else 
      {
        std::cout << "Token detected (identifier) from address " << (void*)fptr << " " << (void*)bptr << std::endl;
      }
      // increment fptr and also move bptr to fptr position for new token identification
			fptr += 1; 
			bptr = fptr;
		}
		else if (isOperator(*fptr)) // check if its operator
		{
			std::cout << "Token detected (operator) from address " << (void*)fptr << " " << (void*)bptr << std::endl;
		}
  }
  	std::cout << "Reached sentinel" << std::endl; // when we encounter '\0' then its sentinel
}

int main() {
  inputBuffer("int a, int b;");
  return 0;
}
