#ifndef Parser_H
#define Parser_H


enum RegexTokenType { LITERAL, OPEN_P,CLOSE_P,STAR,PLUS,QUESTION,PIPE,OPEN_BRACKET,CLOSE_BRACKET,DOT,BACKLASH,START_ANCHOR,END_ANCHOR};

struct token {
  char character;
  RegexTokenType tokenType;
  int position;
  bool insideCharacterClass;
  char escapedChar;
};

int countTokens(const char*regex);

int tokeniser(const char *regex,token *resultTokenArray);

bool regexChecker(token *tokenArray, int tokenCount);

RegexTokenType tokenChecker(char token);

#endif