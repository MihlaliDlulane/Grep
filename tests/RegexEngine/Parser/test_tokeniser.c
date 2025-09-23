#include "../../../RegexEngine/Parser/Parser.H"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

int test_empty_regex() {
  const char *regex = "";
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 0);
  free(tokens);
  return ok;
}

int test_single_char() {
  const char *regex = "a";
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 1 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL);
  free(tokens);
  return ok;
}

int test_single_special() {
  const char *regex = "*";
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 1 && tokens[0].character == '*' &&
            tokens[0].tokenType == STAR);
  free(tokens);
  return ok;
}

const char *tokenNames[] = {
    "LITERAL",  "OPEN_P",       "CLOSE_P",      "STAR",          "PLUS",
    "QUESTION", "PIPE",         "OPEN_BRACKET", "CLOSE_BRACKET", "DOT",
    "BACKLASH", "START_ANCHOR", "END_ANCHOR"};

int test_start_anchor() {
  int ok = 1;

  // Basic start anchor
  const char *regex1 = "^abc";
  token *tokens1 = malloc(4 * sizeof(token));
  int tokenCount1 = tokeniser(regex1, tokens1);
  ok &= (tokenCount1 == 4 && tokens1[0].character == '^' &&
         tokens1[0].tokenType == START_ANCHOR && tokens1[1].character == 'a' &&
         tokens1[1].tokenType == LITERAL && tokens1[2].character == 'b' &&
         tokens1[2].tokenType == LITERAL && tokens1[3].character == 'c' &&
         tokens1[3].tokenType == LITERAL);
  free(tokens1);

  // Start anchor after alternation
  const char *regex2 = "abc|^def";
  token *tokens2 = malloc(8 * sizeof(token));
  int tokenCount2 = tokeniser(regex2, tokens2);
  ok &= (tokenCount2 == 8 && tokens2[0].character == 'a' &&
         tokens2[0].tokenType == LITERAL && tokens2[1].character == 'b' &&
         tokens2[1].tokenType == LITERAL && tokens2[2].character == 'c' &&
         tokens2[2].tokenType == LITERAL && tokens2[3].character == '|' &&
         tokens2[3].tokenType == PIPE && tokens2[4].character == '^' &&
         tokens2[4].tokenType == START_ANCHOR && tokens2[5].character == 'd' &&
         tokens2[5].tokenType == LITERAL && tokens2[6].character == 'e' &&
         tokens2[6].tokenType == LITERAL && tokens2[7].character == 'f' &&
         tokens2[7].tokenType == LITERAL);
  if (ok == 0) {
    printf(RED "Start anchor after altertnation test failed.\n\n" RESET);
  }
  free(tokens2);

  // Start anchor after group
  const char *regex3 = "(abc)^def";
  token *tokens3 = malloc(9 * sizeof(token));
  int tokenCount3 = tokeniser(regex3, tokens3);
  ok &= (tokenCount3 == 9 && tokens3[0].character == '(' &&
         tokens3[0].tokenType == OPEN_P && tokens3[1].character == 'a' &&
         tokens3[1].tokenType == LITERAL && tokens3[2].character == 'b' &&
         tokens3[2].tokenType == LITERAL && tokens3[3].character == 'c' &&
         tokens3[3].tokenType == LITERAL && tokens3[4].character == ')' &&
         tokens3[4].tokenType == CLOSE_P && tokens3[5].character == '^' &&
         tokens3[5].tokenType == LITERAL && tokens3[6].character == 'd' &&
         tokens3[6].tokenType == LITERAL && tokens3[7].character == 'e' &&
         tokens3[7].tokenType == LITERAL && tokens3[8].character == 'f' &&
         tokens3[8].tokenType == LITERAL);

  free(tokens3);

  return ok;
}

int test_parser_basic() {
  const char *regex = "(a|b)*c";
  token *tokens = malloc(7 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 7 && tokens[0].character == '(' &&
            tokens[0].tokenType == OPEN_P && tokens[1].character == 'a' &&
            tokens[1].tokenType == LITERAL && tokens[2].character == '|' &&
            tokens[2].tokenType == PIPE && tokens[3].character == 'b' &&
            tokens[3].tokenType == LITERAL && tokens[4].character == ')' &&
            tokens[4].tokenType == CLOSE_P && tokens[5].character == '*' &&
            tokens[5].tokenType == STAR && tokens[6].character == 'c' &&
            tokens[6].tokenType == LITERAL);
  free(tokens);
  return ok;
}

int main() {
  struct {
    const char *name;
    int (*func)();
  } tests[] = {
      {"test_empty_regex", test_empty_regex},
      {"test_single_char", test_single_char},
      {"test_single_special", test_single_special},
      {"test_start_anchor", test_start_anchor},
      {"test_parser_basic", test_parser_basic},
  };

  printf(YELLOW "Running parser tests...\n\n" RESET);
  int total = sizeof(tests) / sizeof(tests[0]);
  int passed = 0;

  for (int i = 0; i < total; i++) {
    printf(YELLOW "Running %s...\n" RESET, tests[i].name);
    if (tests[i].func()) {
      printf(GREEN "%s PASSED ✅\n\n" RESET, tests[i].name);
      passed++;
    } else {
      printf(RED "%s FAILED ❌\n\n" RESET, tests[i].name);
    }
  }

  if (passed == total) {
    printf(GREEN "All %d/%d parser tests passed! 🎉\n" RESET, passed, total);
  } else {
    printf(RED "%d/%d parser tests passed. Some tests failed.\n" RESET, passed,
           total);
  }

  return (passed == total) ? 0 : 1;
}
