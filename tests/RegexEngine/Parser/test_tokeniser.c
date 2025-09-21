#include "../../../RegexEngine/Parser/Parser.H"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_empty_regex() {
  const char *regex = "";
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  assert(tokenCount == 0);
}

void test_sinlge_char() {
  const char *regex = "a";
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  assert(tokenCount == 1);
  assert(tokens[0].character == 'a' && tokens[0].tokenType == LITERAL);
}

void test_single_special() {
  const char *regex = "*";
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  assert(tokenCount == 1);
  assert(tokens[0].character == '*' && tokens[0].tokenType == STAR);
}

void test_start_anchor() {
  // Basic start anchor
  const char *regex1 = "^abc";
  token *tokens1 = malloc(4 * sizeof(token));
  int tokenCount1 = tokeniser(regex1, tokens1);
  assert(tokenCount1 == 4);
  assert(tokens1[0].character == '^' && tokens1[0].tokenType == START_ANCHOR);
  assert(tokens1[1].character == 'a' && tokens1[1].tokenType == LITERAL);
  assert(tokens1[2].character == '^' && tokens1[2].tokenType == LITERAL);
  assert(tokens1[3].character == '^' && tokens1[3].tokenType == LITERAL);

  // Start anchor after alternation
  const char *regex2 = "abc|^def";
  token *tokens2 = malloc(8 * sizeof(token));
  int tokenCount2 = tokeniser(regex2, tokens2);

  assert(tokenCount2 == 8);
  assert(tokens2[0].character == 'a' && tokens2[0].tokenType == LITERAL);
  assert(tokens2[1].character == 'b' && tokens2[1].tokenType == LITERAL);
  assert(tokens2[2].character == 'c' && tokens2[2].tokenType == LITERAL);
  assert(tokens2[3].character == '|' && tokens2[3].tokenType == PIPE);
  assert(tokens2[4].character == '^' && tokens2[4].tokenType == START_ANCHOR);
  assert(tokens2[5].character == 'd' && tokens2[5].tokenType == LITERAL);
  assert(tokens2[6].character == 'e' && tokens2[6].tokenType == LITERAL);
  assert(tokens2[7].character == 'f' && tokens2[7].tokenType == LITERAL);

  // Start anchor after group
  const char *regex3 = "(abc)^def";
  token *tokens3 = malloc(9 * sizeof(token));
  int tokenCount3 = tokeniser(regex3, tokens3);

  assert(tokenCount3 == 9);
  assert(tokens3[0].character == '(' && tokens3[0].tokenType == OPEN_BRACKET);
  assert(tokens3[1].character == 'a' && tokens3[1].tokenType == LITERAL);
  assert(tokens3[2].character == 'b' && tokens3[2].tokenType == LITERAL);
  assert(tokens3[3].character == 'c' && tokens3[3].tokenType == LITERAL);
  assert(tokens3[4].character == ')' && tokens3[4].tokenType == CLOSE_BRACKET);
  assert(tokens3[5].character == '^' && tokens3[5].tokenType == LITERAL);
  assert(tokens3[6].character == 'd' && tokens3[6].tokenType == LITERAL);
  assert(tokens3[7].character == 'e' && tokens3[7].tokenType == LITERAL);
  assert(tokens3[8].character == 'f' && tokens3[8].tokenType == LITERAL);
}

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

  printf("\n");
  printf("Running emtpty regex test...\n");
  test_empty_regex();
  printf("Passed empty regex passed.\n");

  printf("\n");
  printf("Running single char test...\n");
  test_sinlge_char();
  printf("Passed single char test.\n");

  printf("\n");
  printf("Running single special test...\n");
  test_single_special();
  printf("Passed single special test.\n");

  printf("\n");
  printf("Running start anchor test...\n");
  test_start_anchor();
  printf("Passed start anchor test.\n");

  printf("\n");
  printf("Running pasrer basic test..\n");
  test_parser_basic();
  printf("Passed pasrer basic test.\n");

  printf("\n");
  printf("All parser tests passed!\n");
  return 0;
}
