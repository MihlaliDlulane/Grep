#include "Parser.H"
#include <cstdlib>
#include <cstring>
#include <stdio.h>


bool regexChecker(const char *regex) {

  
  char stack[100];
  int top = -1;
  char currentChar;

  for (int i = 0; regex[i] != '\0'; i++) {
    currentChar = regex[i];

    if (i == 0) {
      if (currentChar == '*' || currentChar == '|') {
        return false;
      }
    }

    if (currentChar == '*' || currentChar == '|') {
      if (regex[i + 1] == '*' || regex[i + 1] == '|' || regex[i + 1] == '(') {
        return false;
      }
    }

    if (currentChar == '(') {

      if (regex[i + 1] == '*' || regex[i + 1] == '|') {
        return false;
      }

      if (top >= 99) {
        printf("Stack Overflow On Parantheses Balance Check\n");
        return false;
      }
      stack[++top] = currentChar;

    } else if (currentChar == ')') {

      if (top == -1) {
        return false; // Unbalanced: closing parenthesis without opening
      }

      char poppedChar;
      if (top < 0) {
        poppedChar = '\0'; // Indicate Empty Stack
      } else {
        poppedChar = stack[top--];
      }
      if ((currentChar == ')' && poppedChar != '(')) {
        return false; // Unbalanced: mismatching parentheses
      }
    }
  }

  if (top != -1) {
    return false;
  }

  char lastChar = regex[strlen(regex) - 1];
  if (lastChar == '|' || lastChar == '*') {
    return false;
  }

    return true;
}

int countTokens(const char *regex) {}

void tokeniser(const char *regex, token *ResultTokenArray) {}