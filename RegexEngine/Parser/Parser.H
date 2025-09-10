#ifndef Parser_H
#define Parser_H

enum operatorType{KLEEN_STAR,CONCATENTAION,ALTERNATION};

enum RegexTokenType { LITERAL, OPERATOR, OPEN_P, CLOSE_P };

struct token {
  char character;
  RegexTokenType tokenType;
};

int countTokens(const char*regex);

void tokeniser(const char *regex,token *resultTokenArray);

bool regexChecker(const char *regex);


#endif