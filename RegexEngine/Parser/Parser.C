#include "Parser.H"
#include <cstddef>
#include <cstring>

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

void tokeniser(const char *regex, token *ResultTokenArray) {
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
        if (regex[i - 1] == '(' || regex[i - 1] == '|' ) {
          currentCharType = START_ANCHOR;
        }
        if (regex[i - 1] == '[' && !insideCharClass) {
          currentCharType = START_ANCHOR;
        }
      }
    }
    
    if (regex[i + 1] != '\0') {
      if(currentChar == '$'){
        if(regex[i+1] == ')' || regex[i+1] == '|' || regex[i+1] == ']'){
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

    struct token t1;
    t1.character = currentChar;
    t1.tokenType = currentCharType;
    t1.position = originalPos;

    if(insideCharClass && currentCharType != OPEN_BRACKET && currentCharType != CLOSE_BRACKET) {
      t1.insideCharacterClass = insideCharClass;
    } else {
      t1.insideCharacterClass = false;
    }
    

    if (currentCharType == BACKLASH) {
      t1.escapedChar = nextChar;
    }

    ResultTokenArray[assignCount] = t1;
    assignCount++;
  }
}

// bool regexChecker(const char *regex) {

// if (strlen(regex) == 0)
//   return true;

// char stack[100];
// int top = -1;
// char currentChar;

// for (int i = 0; regex[i] != '\0'; i++) {
//   currentChar = regex[i];

//   if (i == 0) {
//     if (currentChar == '*' || currentChar == '|') {
//       return false;
//     }
//   }

//   if (regex[i + 1] != '\0') {
//     if (currentChar == '*' || currentChar == '|') {
//       if (regex[i + 1] == '*' || regex[i + 1] == '|' || regex[i + 1] == '(')
//       {
//         return false;
//       }
//     }
//   }

//   if (currentChar == '(') {

//     if (regex[i + 1] != '\0') {
//       if (regex[i + 1] == '*' || regex[i + 1] == '|' || regex[i + 1] == ')')
//       {
//         return false;
//       }
//     }

//     if (top >= 99) {
//       printf("Stack Overflow On Parantheses Balance Check\n");
//       return false;
//     }
//     stack[++top] = currentChar;

//   } else if (currentChar == ')') {

//     if (i > 0) {
//       if (regex[i - 1] == '|') {
//         return false;
//       }
//     }

//     if (regex[i + 1] != '\0') {
//       if (regex[i + 1] == '(') {
//         return false;
//       }
//     }

//     if (top < 1 || stack[top--] != '(') {
//       return false;
//     }
//   }
// }

// if (top != -1) {
//   return false;
// }

// char lastChar = regex[strlen(regex) - 1];
// if (lastChar == '|' || lastChar == '*') {
//   return false;
// }

// return true;
//}
