#include <stdio.h>
#include <string.h>

void scanOnce(char *input)
{
  /* Checking if the token is an alpha numeric char -> if its within ascii range of 97-122
   * for small letters and 65-90 for capital letters then that means its an identifier.
   */
  if (((int)*input >= 97 && (int)*input <= 122) || ((int)*input >= 65 && (int)*input <=90))
  {
    printf("%c IDENTIFIER , ADDRESS: %p\n", *input, input);
  }
  // Checking if the input is an operator
  else if (*input == '=' || *input == '+' || *input == '-' || *input == '*' || *input == '/')
  {
    printf("%c OPERATOR , ADDRESS: %p\n", *input, input);
  }
  // Checking if the input is a digit
  else if (*input >= '0' && *input <= '9')
  {
    printf("%c INTEGER , ADDRESS: %p\n", *input, input);
  }
  // Checking if the input is a punctation mark
  else if (*input == ';' || *input == ',')
  {
    printf("%c PUNCTUATION , ADDRESS: %p\n", *input, input);
  }
}

// I took cli arguments for testing easily instead of compiling everytime for new input
int main(int argc, char** argv) 
{
  char* str = "a + b = c;"; // input str if not cli is given
  // checks if cli argument is given since first cli arg is always program name and it given length will be more than 2
  if (argc > 2)
  {
    str = *(argv + 1);
  }
  // performs lex analysis for every input char in the string
  for (int i = 0; i < strlen(str); i++)
  {
    scanOnce(&str[i]);
  }
  return 0;
}
