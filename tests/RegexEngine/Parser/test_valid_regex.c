#include "../../../RegexEngine/Parser/Parser.H"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

const char *tokenNames[] = {"LITERAL",       "OPEN_P",
                            "CLOSE_P",       "STAR",
                            "PLUS",          "QUESTION",
                            "PIPE",          "OPEN_BRACKET",
                            "CLOSE_BRACKET", "DOT",
                            "BACKLASH",      "START_ANCHOR",
                            "END_ANCHOR",    "CHAR_CLASS_NEGATION",
                            "SEQUENCE"};

int test_valid_basic() {
  int ok = 1;

  // Simple literals
  const char *regex = "abc";
  token *tokens = malloc(3 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  if (!(tokenCount == 3 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, regex);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex2 = "a";
  tokens = malloc(1 * sizeof(token));
  tokenCount = tokeniser(regex2, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, regex2);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex3 = "";
  tokens = malloc(1 * sizeof(token)); // Allocate minimal memory for empty case
  tokenCount = tokeniser(regex3, tokens);
  if (!(tokenCount == 0 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, regex3);
    printf("Token count: %d (expected 0)\n", tokenCount);
  }
  free(tokens);

  // Basic operators
  const char *op_regex = "a*";
  tokens = malloc(2 * sizeof(token));
  tokenCount = tokeniser(op_regex, tokens);
  if (!(tokenCount == 2 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, op_regex);
    printf("Token count: %d (expected 2)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 2; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *op_regex2 = "a+";
  tokens = malloc(2 * sizeof(token));
  tokenCount = tokeniser(op_regex2, tokens);
  if (!(tokenCount == 2 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, op_regex2);
    printf("Token count: %d (expected 2)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 2; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *op_regex3 = "a?";
  tokens = malloc(2 * sizeof(token));
  tokenCount = tokeniser(op_regex3, tokens);
  if (!(tokenCount == 2 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, op_regex3);
    printf("Token count: %d (expected 2)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 2; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *op_regex5 = "a|b";
  tokens = malloc(3 * sizeof(token));
  tokenCount = tokeniser(op_regex5, tokens);
  if (!(tokenCount == 3 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, op_regex5);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *op_regex6 = "ab*c";
  tokens = malloc(4 * sizeof(token));
  tokenCount = tokeniser(op_regex6, tokens);
  if (!(tokenCount == 4 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_basic failed for regex: %s\n" RESET, op_regex6);
    printf("Token count: %d (expected 4)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 4; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  return ok;
}
int test_valid_groups() {
  int ok = 1;

  const char *regex1 = "(a)";
  token *tokens = malloc(3 * sizeof(token));
  int tokenCount = tokeniser(regex1, tokens);
  if (!(tokenCount == 3 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_groups failed for regex: %s\n" RESET, regex1);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex2 = "(abc)";
  tokens = malloc(5 * sizeof(token));
  tokenCount = tokeniser(regex2, tokens);
  if (!(tokenCount == 5 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_groups failed for regex: %s\n" RESET, regex2);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex3 = "(a|b)";
  tokens = malloc(5 * sizeof(token));
  tokenCount = tokeniser(regex3, tokens);
  if (!(tokenCount == 5 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_groups failed for regex: %s\n" RESET, regex3);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex4 = "((a))";
  tokens = malloc(5 * sizeof(token));
  tokenCount = tokeniser(regex4, tokens);
  if (!(tokenCount == 5 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_groups failed for regex: %s\n" RESET, regex4);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex5 = "(a)(b)";
  tokens = malloc(6 * sizeof(token));
  tokenCount = tokeniser(regex5, tokens);
  if (!(tokenCount == 6 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_groups failed for regex: %s\n" RESET, regex5);
    printf("Token count: %d (expected 6)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 6; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex6 = "(a|b)*";
  tokens = malloc(6 * sizeof(token));
  tokenCount = tokeniser(regex6, tokens);
  if (!(tokenCount == 6 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_groups failed for regex: %s\n" RESET, regex6);
    printf("Token count: %d (expected 6)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 6; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex7 = "(a*b+)?";
  tokens = malloc(7 * sizeof(token));
  tokenCount = tokeniser(regex7, tokens);
  if (!(tokenCount == 7 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_groups failed for regex: %s\n" RESET, regex7);
    printf("Token count: %d (expected 7)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 7; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  return ok;
}

int test_valid_char_classes() {
  int ok = 1;

  const char *regex1 = "[a]";
  token *tokens = malloc(3 * sizeof(token));
  int tokenCount = tokeniser(regex1, tokens);
  if (!(tokenCount == 3 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex1);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex2 = "[abc]";
  tokens = malloc(5 * sizeof(token));
  tokenCount = tokeniser(regex2, tokens);
  if (!(tokenCount == 5 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex2);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex3 = "[a-z]";
  tokens = malloc(5 * sizeof(token));
  tokenCount = tokeniser(regex3, tokens);
  if (!(tokenCount == 5 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex3);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex4 = "[^abc]";
  tokens = malloc(6 * sizeof(token));
  tokenCount = tokeniser(regex4, tokens);
  if (!(tokenCount == 6 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex4);
    printf("Token count: %d (expected 6)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 6; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex5 = "[a-zA-Z0-9]";
  tokens = malloc(11 * sizeof(token));
  tokenCount = tokeniser(regex5, tokens);
  if (!(tokenCount == 11 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex5);
    printf("Token count: %d (expected 11)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 11; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex6 = "[]";
  tokens = malloc(2 * sizeof(token));
  tokenCount = tokeniser(regex6, tokens);
  if (!(tokenCount == 2 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex6);
    printf("Token count: %d (expected 2)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 2; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex7 = "[\\]]";
  tokens = malloc(3 * sizeof(token));
  tokenCount = tokeniser(regex7, tokens);
  if (!(tokenCount == 3 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex7);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex8 = "[\\[]";
  tokens = malloc(3 * sizeof(token));
  tokenCount = tokeniser(regex8, tokens);
  if (!(tokenCount == 3 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_char_classes failed for regex: %s\n" RESET, regex8);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  return ok;
}

int test_valid_anchors() {
  int ok = 1;

  const char *regex1 = "^abc";
  token *tokens = malloc(4 * sizeof(token));
  int tokenCount = tokeniser(regex1, tokens);
  if (!(tokenCount == 4 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_anchors failed for regex: %s\n" RESET, regex1);
    printf("Token count: %d (expected 4)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 4; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex2 = "abc$";
  tokens = malloc(4 * sizeof(token));
  tokenCount = tokeniser(regex2, tokens);
  if (!(tokenCount == 4 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_anchors failed for regex: %s\n" RESET, regex2);
    printf("Token count: %d (expected 4)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 4; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex3 = "^abc$";
  tokens = malloc(5 * sizeof(token));
  tokenCount = tokeniser(regex3, tokens);
  if (!(tokenCount == 5 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_anchors failed for regex: %s\n" RESET, regex3);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex4 = "^$";
  tokens = malloc(2 * sizeof(token));
  tokenCount = tokeniser(regex4, tokens);
  if (!(tokenCount == 2 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_anchors failed for regex: %s\n" RESET, regex4);
    printf("Token count: %d (expected 2)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 2; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex5 = "^a|b$";
  tokens = malloc(5 * sizeof(token));
  tokenCount = tokeniser(regex5, tokens);
  if (!(tokenCount == 5 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_anchors failed for regex: %s\n" RESET, regex5);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex6 = "(^abc)|(def$)";
  tokens = malloc(10 * sizeof(token));
  tokenCount = tokeniser(regex6, tokens);
  if (!(tokenCount == 10 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_anchors failed for regex: %s\n" RESET, regex6);
    printf("Token count: %d (expected 10)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 10; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  return ok;
}

int test_valid_escapes() {
  int ok = 1;

  const char *regex1 = "\\*";
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex1, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex1);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex2 = "\\+";
  tokens = malloc(1 * sizeof(token));
  tokenCount = tokeniser(regex2, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex2);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex3 = "\\?";
  tokens = malloc(1 * sizeof(token));
  tokenCount = tokeniser(regex3, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex3);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex4 = "\\$";
  tokens = malloc(1 * sizeof(token));
  tokenCount = tokeniser(regex4, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex4);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex5 = "\\[\\]";
  tokens = malloc(3 * sizeof(token));
  tokenCount = tokeniser(regex5, tokens);
  if (!(tokenCount == 3 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex5);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex6 = "\\\\";
  tokens = malloc(1 * sizeof(token));
  tokenCount = tokeniser(regex6, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex6);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex7 = "\\n";
  tokens = malloc(1 * sizeof(token));
  tokenCount = tokeniser(regex7, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex7);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex8 = "\\t";
  tokens = malloc(1 * sizeof(token));
  tokenCount = tokeniser(regex8, tokens);
  if (!(tokenCount == 1 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_escapes failed for regex: %s\n" RESET, regex8);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  return ok;
}

int test_valid_complex() {
  int ok = 1;

  const char *regex1 = "(a|b)*abb";
  token *tokens = malloc(8 * sizeof(token));
  int tokenCount = tokeniser(regex1, tokens);
  if (!(tokenCount == 8 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_complex failed for regex: %s\n" RESET, regex1);
    printf("Token count: %d (expected 8)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 8; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  const char *regex2 = "^[a-zA-Z]+@[a-zA-Z]+\\.[a-zA-Z]+$";
  tokens = malloc(27 * sizeof(token));
  tokenCount = tokeniser(regex2, tokens);
  if (!(tokenCount == 27 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_complex failed for regex: %s\n" RESET, regex2);
    printf("Token count: %d (expected 27)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 27; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex3 = "\\d{3}-\\d{3}-\\d{4}";
  tokens = malloc(13 * sizeof(token));
  tokenCount = tokeniser(regex3, tokens);
  if (!(tokenCount == 13 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_complex failed for regex: %s\n" RESET, regex3);
    printf("Token count: %d (expected 13)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 13; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex4 = "(cat|dog)s?";
  tokens = malloc(10 * sizeof(token));
  tokenCount = tokeniser(regex4, tokens);
  if (!(tokenCount == 10 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_complex failed for regex: %s\n" RESET, regex4);
    printf("Token count: %d (expected 10)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 10; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);

  const char *regex5 = "https?://";
  tokens = malloc(7 * sizeof(token));
  tokenCount = tokeniser(regex5, tokens);
  if (!(tokenCount == 7 && regexChecker(tokens, tokenCount))) {
    ok = 0;
    printf(RED "test_valid_complex failed for regex: %s\n" RESET, regex5);
    printf("Token count: %d (expected 7)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 7; i++) {
      printf("token %d, character: %c, type: %s%s\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? ", insideCharacterClass: true"
                                            : "");
    }
  }
  free(tokens);

  return ok;
}

int main() {
  struct {
    const char *name;
    int (*func)();
  } tests[] = {{"test_valid_basic", test_valid_basic},
               {"test_valid_groups", test_valid_groups},
               {"test_valid_char_classes", test_valid_char_classes},
               {"test_valid_anchors", test_valid_anchors},
               {"test_valid_escapes", test_valid_escapes},
               {"test_valid_complex", test_valid_complex}};

  printf(YELLOW "Running regex validation tests...\n\n" RESET);
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
    printf(GREEN "All %d/%d regex validation tests passed! 🎉\n" RESET, passed,
           total);
  } else {
    printf(RED
           "%d/%d regex validation tests passed. Some tests failed.\n" RESET,
           passed, total);
  }

  return (passed == total) ? 0 : 1;
}
