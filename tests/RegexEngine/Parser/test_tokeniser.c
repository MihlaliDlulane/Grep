#include "../../../RegexEngine/Parser/Parser.H"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_parser_basic() {
  const char *regex = "(a|b)*c";
  token *tokens = malloc(7 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  assert(tokenCount == 7);
  assert(tokens[0].character == '(' && tokens[0].tokenType == OPEN_P);
  assert(tokens[1].character == 'a' && tokens[1].tokenType == LITERAL);
  assert(tokens[2].character == '|' && tokens[2].tokenType == PIPE);
  assert(tokens[3].character == 'b' && tokens[3].tokenType == LITERAL);
  assert(tokens[4].character == ')' && tokens[4].tokenType == CLOSE_P);
  assert(tokens[5].character == '*' && tokens[5].tokenType == STAR);
  assert(tokens[6].character == 'c' && tokens[6].tokenType == LITERAL);
  free(tokens);
}

int main() {
  printf("Running parser tests...\n");
  test_parser_basic();
  printf("All parser tests passed!\n");
  return 0;
}
