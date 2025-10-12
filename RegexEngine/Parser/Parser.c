#include "Parser.H"
#include <string.h>

RegexTokenType tokenChecker(char token) {
  if (token == '(') {
    return OPEN_P;
  }

  if (token == ')') {
    return CLOSE_P;
  }

  if (token == '*') {
    return STAR;
  }

  if (token == '+') {
    return PLUS;
  }

  if (token == '|') {
    return PIPE;
  }

  if (token == '?') {
    return QUESTION;
  }

  if (token == '[') {
    return OPEN_BRACKET;
  }

  if (token == ']') {
    return CLOSE_BRACKET;
  }

  if (token == '.') {
    return DOT;
  }

  if (token == '\\') {
    return BACKLASH;
  }

  return LITERAL;
}

int tokeniser(const char *regex, token *ResultTokenArray) {

  int regexLength = strlen(regex);
  int assignCount = 0;
  bool insideCharClass = false;
  for (int i = 0; regex[i] != '\0'; i++) {
    int originalPos = i;
    char currentChar = regex[i];
    char nextChar = '\0';
    RegexTokenType currentCharType = tokenChecker(currentChar);

    if (currentCharType == OPEN_BRACKET) {
      insideCharClass = true;
    }

    if (currentCharType == CLOSE_BRACKET) {
      insideCharClass = false;
    }

    if (i == 0) {
      if (currentChar == '^') {
        currentCharType = START_ANCHOR;
      }
    }

    if (i > 0) {
      if (currentChar == '^') {
        if (regex[i - 1] == '(' || regex[i - 1] == '|') {
          currentCharType = START_ANCHOR;
        }
      }
    }

    if (regex[i + 1] != '\0') {
      if (currentChar == '$') {
        if (regex[i + 1] == ')' || regex[i + 1] == '|' || regex[i + 1] == ']') {
          currentCharType = END_ANCHOR;
        }
      }

      if (i == regexLength - 1) {
        if (currentChar == '$') {
          currentCharType = END_ANCHOR;
        }
      }

      if (currentCharType == BACKLASH) {
        if (regex[i + 1] != '\0') {
          nextChar = regex[i + 1];
        }
        i++;
      }
    }

    if (insideCharClass) {
      if (currentCharType == STAR || currentCharType == QUESTION ||
          currentCharType == PLUS) {
        currentCharType = LITERAL;
      }

      if (currentChar == '^') {
        currentCharType = CHAR_CLASS_NEGATION;
      }
    }

    token t1;
    t1.character = currentChar;
    t1.tokenType = currentCharType;
    t1.position = originalPos;

    if (insideCharClass && currentCharType != OPEN_BRACKET &&
        currentCharType != CLOSE_BRACKET) {
      t1.insideCharacterClass = insideCharClass;
    } else {
      t1.insideCharacterClass = false;
    }

    if (currentCharType == BACKLASH) {
      t1.escapedChar = nextChar;
      t1.escapedCharType = LITERAL;

      if (nextChar == 'd' || nextChar == 'w' || nextChar == 's' ||
          nextChar == 'n' || nextChar == 't' || nextChar == 'b') {
        t1.escapedCharType = SEQUENCE;
      }
    }

    ResultTokenArray[assignCount] = t1;
    assignCount++;
  }
  return assignCount;
}

bool regexChecker(token *tokenArray, int tokenCount) { return 0; }
