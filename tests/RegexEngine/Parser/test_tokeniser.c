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

int test_char_class_basic() {
  const char *regex = "[abc]";
  // Check insideCharacterClass flags
  // [ -> false, a -> true, b -> true, c -> true, ] -> true
  token *tokens = malloc(5 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 5 && tokens[0].character == '[' &&
            tokens[0].tokenType == OPEN_BRACKET && tokens[1].character == 'a' &&
            tokens[1].tokenType == LITERAL && tokens[1].insideCharacterClass &&
            tokens[2].character == 'b' && tokens[2].tokenType == LITERAL &&
            tokens[2].insideCharacterClass && tokens[3].character == 'c' &&
            tokens[3].tokenType == LITERAL && tokens[3].insideCharacterClass &&
            tokens[4].character == ']' && tokens[4].tokenType == CLOSE_BRACKET);
  free(tokens);

  return ok;
}

int test_char_class_negation() {
  const char *regex = "[^abc]";
  // ^ after [ should be negation
  token *tokens = malloc(6 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 6 && tokens[0].character == '[' &&
            tokens[0].tokenType == OPEN_BRACKET && tokens[1].character == '^' &&
            tokens[1].tokenType == CHAR_CLASS_NEGATION &&
            tokens[1].insideCharacterClass && tokens[2].character == 'a' &&
            tokens[2].tokenType == LITERAL && tokens[2].insideCharacterClass &&
            tokens[3].character == 'b' && tokens[3].tokenType == LITERAL &&
            tokens[3].insideCharacterClass && tokens[4].character == 'c' &&
            tokens[4].tokenType == LITERAL && tokens[5].character == ']' &&
            tokens[5].tokenType == CLOSE_BRACKET);

  free(tokens);

  return ok;
}

int test_char_class_special_chars() {
  const char *regex = "[*+?]";
  // Special chars are literals inside []

  token *tokens = malloc(5 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 5 && tokens[0].character == '[' &&
            tokens[0].tokenType == OPEN_BRACKET && tokens[1].character == '*' &&
            tokens[1].tokenType == LITERAL && tokens[1].insideCharacterClass &&
            tokens[2].character == '+' && tokens[2].tokenType == LITERAL &&
            tokens[2].insideCharacterClass && tokens[3].character == '?' &&
            tokens[3].tokenType == LITERAL && tokens[3].insideCharacterClass &&
            tokens[4].character == ']' && tokens[4].tokenType == CLOSE_BRACKET);
  if (ok == -1) {
    printf("token 1, character: %c and type:%s \n", tokens[0].character,
           tokenNames[tokens[0].tokenType]);
    printf("token 2, character: %c and type:%s \n", tokens[1].character,
           tokenNames[tokens[1].tokenType]);
    printf("token 3, character: %c and type:%s \n", tokens[2].character,
           tokenNames[tokens[2].tokenType]);
    printf("token 4, character: %c and type:%s \n", tokens[3].character,
           tokenNames[tokens[3].tokenType]);
    printf("token 5, character: %c and type:%s \n", tokens[4].character,
           tokenNames[tokens[4].tokenType]);
  }

  free(tokens);

  return ok;
}
int test_char_class_escaped() {
  const char *regex = "[\\]]";
  // Escaped ] inside character class
  token *tokens = malloc(3 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 3 && tokens[0].character == '[' &&
            tokens[0].tokenType == OPEN_BRACKET &&
            tokens[1].character == '\\' && tokens[1].escapedChar == ']' &&
            tokens[1].tokenType == BACKLASH && tokens[1].insideCharacterClass &&
            tokens[2].character == ']' && tokens[2].tokenType == CLOSE_BRACKET);

  if (ok != 1) {
    printf("Token count: %i \n", tokenCount);
    printf("token 1 , character: %c and type: %s \n", tokens[0].character,
           tokenNames[tokens[0].tokenType]);
    printf(
        "token 2, character: %c and escapedChar: %c and character type: %s \n",
        tokens[1].character, tokens[1].escapedChar,
        tokenNames[tokens[1].tokenType]);
    printf("token 3,character: %c and type: %s \n", tokens[2].character,
           tokenNames[tokens[2].tokenType]);
  }

  free(tokens);
  return ok;
}
int test_quantifiers() {
  const char *regex = "a*b+c?";
  // Test *, +, ? quantifiers
  token *tokens = malloc(6 * sizeof(token)); // 6 tokens: a, *, b, +, c, ?
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 6 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL && tokens[1].character == '*' &&
            tokens[1].tokenType == STAR && tokens[2].character == 'b' &&
            tokens[2].tokenType == LITERAL && tokens[3].character == '+' &&
            tokens[3].tokenType == PLUS && tokens[4].character == 'c' &&
            tokens[4].tokenType == LITERAL && tokens[5].character == '?' &&
            tokens[5].tokenType == QUESTION);

  if (ok != 1) {
    printf(RED "test_quantifiers failed:\n" RESET);
    printf("Token count: %d (expected 6)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 6; i++) {
      printf("token %d, character: %c, type: %s\n", i + 1, tokens[i].character,
             tokenNames[tokens[i].tokenType]);
    }
  }

  free(tokens);
  return ok;
}

int test_quantifier_on_group() {
  const char *regex = "(abc)*";
  // Quantifier applies to group
  token *tokens = malloc(6 * sizeof(token)); // 6 tokens: (, a, b, c, ), *
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 6 && tokens[0].character == '(' &&
            tokens[0].tokenType == OPEN_P && tokens[1].character == 'a' &&
            tokens[1].tokenType == LITERAL && tokens[2].character == 'b' &&
            tokens[2].tokenType == LITERAL && tokens[3].character == 'c' &&
            tokens[3].tokenType == LITERAL && tokens[4].character == ')' &&
            tokens[4].tokenType == CLOSE_P && tokens[5].character == '*' &&
            tokens[5].tokenType == STAR);

  if (ok != 1) {
    printf(RED "test_quantifier_on_group failed:\n" RESET);
    printf("Token count: %d (expected 6)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 6; i++) {
      printf("token %d, character: %c, type: %s\n", i + 1, tokens[i].character,
             tokenNames[tokens[i].tokenType]);
    }
  }

  free(tokens);
  return ok;
}

int test_consecutive_quantifiers() {
  const char *regex = "a**";
  // Consecutive quantifiers, tokenized as-is
  token *tokens = malloc(3 * sizeof(token)); // 3 tokens: a, *, *
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 3 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL && tokens[1].character == '*' &&
            tokens[1].tokenType == STAR && tokens[2].character == '*' &&
            tokens[2].tokenType == STAR);

  if (ok != 1) {
    printf(RED "test_consecutive_quantifiers failed:\n" RESET);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, type: %s\n", i + 1, tokens[i].character,
             tokenNames[tokens[i].tokenType]);
    }
  }

  free(tokens);
  return ok;
}

int test_escape_special() {
  const char *regex = "a\\*b";
  // Escaped * should be treated as a literal
  token *tokens = malloc(3 * sizeof(token)); // 3 tokens: a, \, b
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 3 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL && tokens[1].character == '\\' &&
            tokens[1].escapedChar == '*' && tokens[1].tokenType == BACKLASH &&
            tokens[1].escapedCharType == LITERAL &&
            tokens[2].character == 'b' && tokens[2].tokenType == LITERAL);

  if (ok != 1) {
    printf(RED "test_escape_special failed:\n" RESET);
    printf("Token count: %d (expected 3)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 3; i++) {
      printf("token %d, character: %c, escapedChar: %c, type: %s, "
             "escapedCharType: %s\n",
             i + 1, tokens[i].character, tokens[i].escapedChar,
             tokenNames[tokens[i].tokenType],
             tokenNames[tokens[i].escapedCharType]);
    }
  }

  free(tokens);
  return ok;
}

int test_escape_literal() {
  const char *regex = "\\n\\t";
  // Escaped newline and tab
  token *tokens = malloc(2 * sizeof(token)); // 2 tokens: \n, \t
  int tokenCount = tokeniser(regex, tokens);
  int ok =
      (tokenCount == 2 && tokens[0].character == '\\' &&
       tokens[0].escapedChar == 'n' && tokens[0].tokenType == BACKLASH &&
       tokens[0].escapedCharType == SEQUENCE && tokens[1].character == '\\' &&
       tokens[1].escapedChar == 't' && tokens[1].tokenType == BACKLASH &&
       tokens[1].escapedCharType == SEQUENCE);

  if (ok != 1) {
    printf(RED "test_escape_literal failed:\n" RESET);
    printf("Token count: %d (expected 2)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 2; i++) {
      printf("token %d, character: %c, escapedChar: %c, type: %s, "
             "escapedCharType: %s\n",
             i + 1, tokens[i].character, tokens[i].escapedChar,
             tokenNames[tokens[i].tokenType],
             tokenNames[tokens[i].escapedCharType]);
    }
  }

  free(tokens);
  return ok;
}

int test_escape_backslash() {
  const char *regex = "\\\\";
  // Escaped backslash should be treated as a literal backslash
  token *tokens = malloc(1 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 1 && tokens[0].character == '\\' &&
            tokens[0].escapedChar == '\\' && tokens[0].tokenType == BACKLASH &&
            tokens[0].escapedCharType == LITERAL);

  if (ok != 1) {
    printf(RED "test_escape_backslash failed:\n" RESET);
    printf("Token count: %d (expected 1)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 1; i++) {
      printf("token %d, character: %c, escapedChar: %c, type: %s, "
             "escapedCharType: %s\n",
             i + 1, tokens[i].character, tokens[i].escapedChar,
             tokenNames[tokens[i].tokenType],
             tokenNames[tokens[i].escapedCharType]);
    }
  }

  free(tokens);
  return ok;
}

int test_escape_end_of_string() {
  const char *regex = "abc\\";
  // Backslash at end, tokenized as BACKLASH with undefined escapedChar
  token *tokens = malloc(4 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 4 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL && tokens[1].character == 'b' &&
            tokens[1].tokenType == LITERAL && tokens[2].character == 'c' &&
            tokens[2].tokenType == LITERAL && tokens[3].character == '\\' &&
            tokens[3].tokenType == BACKLASH && tokens[3].escapedChar == '\0' &&
            tokens[3].escapedCharType == LITERAL);

  if (ok != 1) {
    printf(RED "test_escape_end_of_string failed:\n" RESET);
    printf("Token count: %d (expected 4)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 4; i++) {
      printf("token %d, character: %c, escapedChar: %c, type: %s, "
             "escapedCharType: %s\n",
             i + 1, tokens[i].character, tokens[i].escapedChar,
             tokenNames[tokens[i].tokenType],
             tokenNames[tokens[i].escapedCharType]);
    }
  }

  free(tokens);
  return ok;
}

int test_nested_groups() {
  const char *regex = "((a|b)c)*";
  // Nested parentheses
  token *tokens =
      malloc(9 * sizeof(token)); // 9 tokens: (, (, a, |, b, ), c, ), *
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 9 && tokens[0].character == '(' &&
            tokens[0].tokenType == OPEN_P && tokens[1].character == '(' &&
            tokens[1].tokenType == OPEN_P && tokens[2].character == 'a' &&
            tokens[2].tokenType == LITERAL && tokens[3].character == '|' &&
            tokens[3].tokenType == PIPE && tokens[4].character == 'b' &&
            tokens[4].tokenType == LITERAL && tokens[5].character == ')' &&
            tokens[5].tokenType == CLOSE_P && tokens[6].character == 'c' &&
            tokens[6].tokenType == LITERAL && tokens[7].character == ')' &&
            tokens[7].tokenType == CLOSE_P && tokens[8].character == '*' &&
            tokens[8].tokenType == STAR);

  if (ok != 1) {
    printf(RED "test_nested_groups failed:\n" RESET);
    printf("Token count: %d (expected 9)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 9; i++) {
      printf("token %d, character: %c, type: %s\n", i + 1, tokens[i].character,
             tokenNames[tokens[i].tokenType]);
    }
  }

  free(tokens);
  return ok;
}

int test_alternation() {
  const char *regex = "a|b|c";
  // Multiple alternations
  token *tokens = malloc(5 * sizeof(token)); // 5 tokens: a, |, b, |, c
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 5 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL && tokens[1].character == '|' &&
            tokens[1].tokenType == PIPE && tokens[2].character == 'b' &&
            tokens[2].tokenType == LITERAL && tokens[3].character == '|' &&
            tokens[3].tokenType == PIPE && tokens[4].character == 'c' &&
            tokens[4].tokenType == LITERAL);

  if (ok != 1) {
    printf(RED "test_alternation failed:\n" RESET);
    printf("Token count: %d (expected 5)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 5; i++) {
      printf("token %d, character: %c, type: %s\n", i + 1, tokens[i].character,
             tokenNames[tokens[i].tokenType]);
    }
  }

  free(tokens);
  return ok;
}

int test_empty_alternation() {
  const char *regex = "a|";
  // Empty alternative
  token *tokens = malloc(2 * sizeof(token)); // 2 tokens: a, |
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 2 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL && tokens[1].character == '|' &&
            tokens[1].tokenType == PIPE);

  if (ok != 1) {
    printf(RED "test_empty_alternation failed:\n" RESET);
    printf("Token count: %d (expected 2)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 2; i++) {
      printf("token %d, character: %c, type: %s\n", i + 1, tokens[i].character,
             tokenNames[tokens[i].tokenType]);
    }
  }

  free(tokens);
  return ok;
}

int test_email_pattern() {
  const char *regex = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
  // Real-world pattern: ^[alphanumeric._%+-]+@[alphanumeric.-]+\.[a-zA-Z]{2,}$
  token *tokens = malloc(47 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok =
      (tokenCount == 47 && tokens[0].character == '^' &&
       tokens[0].tokenType == START_ANCHOR && tokens[1].character == '[' &&
       tokens[1].tokenType == OPEN_BRACKET && tokens[2].character == 'a' &&
       tokens[2].tokenType == LITERAL && tokens[2].insideCharacterClass &&
       tokens[3].character == '-' && tokens[3].tokenType == LITERAL &&
       tokens[3].insideCharacterClass && tokens[4].character == 'z' &&
       tokens[4].tokenType == LITERAL && tokens[4].insideCharacterClass &&
       tokens[5].character == 'A' && tokens[5].tokenType == LITERAL &&
       tokens[5].insideCharacterClass && tokens[6].character == '-' &&
       tokens[6].tokenType == LITERAL && tokens[6].insideCharacterClass &&
       tokens[7].character == 'Z' && tokens[7].tokenType == LITERAL &&
       tokens[7].insideCharacterClass && tokens[8].character == '0' &&
       tokens[8].tokenType == LITERAL && tokens[8].insideCharacterClass &&
       tokens[9].character == '-' && tokens[9].tokenType == LITERAL &&
       tokens[9].insideCharacterClass && tokens[10].character == '9' &&
       tokens[10].tokenType == LITERAL && tokens[10].insideCharacterClass &&
       tokens[11].character == '.' && tokens[11].tokenType == DOT &&
       tokens[11].insideCharacterClass && tokens[12].character == '_' &&
       tokens[12].tokenType == LITERAL && tokens[12].insideCharacterClass &&
       tokens[13].character == '%' && tokens[13].tokenType == LITERAL &&
       tokens[13].insideCharacterClass && tokens[14].character == '+' &&
       tokens[14].tokenType == LITERAL && tokens[14].insideCharacterClass &&
       tokens[15].character == '-' && tokens[15].tokenType == LITERAL &&
       tokens[15].insideCharacterClass && tokens[16].character == ']' &&
       tokens[16].tokenType == CLOSE_BRACKET && tokens[17].character == '+' &&
       tokens[17].tokenType == PLUS && tokens[18].character == '@' &&
       tokens[18].tokenType == LITERAL && tokens[19].character == '[' &&
       tokens[19].tokenType == OPEN_BRACKET && tokens[20].character == 'a' &&
       tokens[20].tokenType == LITERAL && tokens[20].insideCharacterClass &&
       tokens[21].character == '-' && tokens[21].tokenType == LITERAL &&
       tokens[21].insideCharacterClass && tokens[22].character == 'z' &&
       tokens[22].tokenType == LITERAL && tokens[22].insideCharacterClass &&
       tokens[23].character == 'A' && tokens[23].tokenType == LITERAL &&
       tokens[23].insideCharacterClass && tokens[24].character == '-' &&
       tokens[24].tokenType == LITERAL && tokens[24].insideCharacterClass &&
       tokens[25].character == 'Z' && tokens[25].tokenType == LITERAL &&
       tokens[25].insideCharacterClass && tokens[26].character == '0' &&
       tokens[26].tokenType == LITERAL && tokens[26].insideCharacterClass &&
       tokens[27].character == '-' && tokens[27].tokenType == LITERAL &&
       tokens[27].insideCharacterClass && tokens[28].character == '9' &&
       tokens[28].tokenType == LITERAL && tokens[28].insideCharacterClass &&
       tokens[29].character == '.' && tokens[29].tokenType == DOT &&
       tokens[29].insideCharacterClass && tokens[30].character == '-' &&
       tokens[30].tokenType == LITERAL && tokens[30].insideCharacterClass &&
       tokens[31].character == ']' && tokens[31].tokenType == CLOSE_BRACKET &&
       tokens[32].character == '+' && tokens[32].tokenType == PLUS &&
       tokens[33].character == '\\' && tokens[33].tokenType == BACKLASH &&
       tokens[33].escapedChar == '.' && tokens[33].escapedCharType == LITERAL &&
       tokens[34].character == '[' && tokens[34].tokenType == OPEN_BRACKET &&
       tokens[35].character == 'a' && tokens[35].tokenType == LITERAL &&
       tokens[35].insideCharacterClass && tokens[36].character == '-' &&
       tokens[36].tokenType == LITERAL && tokens[36].insideCharacterClass &&
       tokens[37].character == 'z' && tokens[37].tokenType == LITERAL &&
       tokens[37].insideCharacterClass && tokens[38].character == 'A' &&
       tokens[38].tokenType == LITERAL && tokens[38].insideCharacterClass &&
       tokens[39].character == '-' && tokens[39].tokenType == LITERAL &&
       tokens[39].insideCharacterClass && tokens[40].character == 'Z' &&
       tokens[40].tokenType == LITERAL && tokens[40].insideCharacterClass &&
       tokens[41].character == ']' && tokens[41].tokenType == CLOSE_BRACKET &&
       tokens[42].character == '{' && tokens[42].tokenType == LITERAL &&
       tokens[43].character == '2' && tokens[43].tokenType == LITERAL &&
       tokens[44].character == ',' && tokens[44].tokenType == LITERAL &&
       tokens[45].character == '}' && tokens[45].tokenType == LITERAL &&
       tokens[46].character == '$' && tokens[46].tokenType == LITERAL);

  if (ok != 1) {
    printf(RED "test_email_pattern failed:\n" RESET);
    printf("Token count: %d (expected 47)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 47; i++) {
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

int test_mixed_contexts() {
  const char *regex = "^(\\d{3})\\s*[.-]?\\s*(\\d{3})\\s*[.-]?\\s*(\\d{4})$";
  // Phone number pattern: ^(\d{3})\s*[.-]?\s*(\d{3})\s*[.-]?\s*(\d{4})$
  token *tokens = malloc(38 * sizeof(token));
  int tokenCount = tokeniser(regex, tokens);
  int ok =
      (tokenCount == 38 && tokens[0].character == '^' &&
       tokens[0].tokenType == START_ANCHOR && tokens[1].character == '(' &&
       tokens[1].tokenType == OPEN_P && tokens[2].character == '\\' &&
       tokens[2].tokenType == BACKLASH && tokens[2].escapedChar == 'd' &&
       tokens[2].escapedCharType == SEQUENCE && tokens[3].character == '{' &&
       tokens[3].tokenType == LITERAL && tokens[4].character == '3' &&
       tokens[4].tokenType == LITERAL && tokens[5].character == '}' &&
       tokens[5].tokenType == LITERAL && tokens[6].character == ')' &&
       tokens[6].tokenType == CLOSE_P && tokens[7].character == '\\' &&
       tokens[7].tokenType == BACKLASH && tokens[7].escapedChar == 's' &&
       tokens[7].escapedCharType == SEQUENCE && tokens[8].character == '*' &&
       tokens[8].tokenType == STAR && tokens[9].character == '[' &&
       tokens[9].tokenType == OPEN_BRACKET && tokens[10].character == '.' &&
       tokens[10].tokenType == DOT && tokens[10].insideCharacterClass &&
       tokens[11].character == '-' && tokens[11].tokenType == LITERAL &&
       tokens[11].insideCharacterClass && tokens[12].character == ']' &&
       tokens[12].tokenType == CLOSE_BRACKET && tokens[13].character == '?' &&
       tokens[13].tokenType == QUESTION && tokens[14].character == '\\' &&
       tokens[14].tokenType == BACKLASH && tokens[14].escapedChar == 's' &&
       tokens[14].escapedCharType == SEQUENCE && tokens[15].character == '*' &&
       tokens[15].tokenType == STAR && tokens[16].character == '(' &&
       tokens[16].tokenType == OPEN_P && tokens[17].character == '\\' &&
       tokens[17].tokenType == BACKLASH && tokens[17].escapedChar == 'd' &&
       tokens[17].escapedCharType == SEQUENCE && tokens[18].character == '{' &&
       tokens[18].tokenType == LITERAL && tokens[19].character == '3' &&
       tokens[19].tokenType == LITERAL && tokens[20].character == '}' &&
       tokens[20].tokenType == LITERAL && tokens[21].character == ')' &&
       tokens[21].tokenType == CLOSE_P && tokens[22].character == '\\' &&
       tokens[22].tokenType == BACKLASH && tokens[22].escapedChar == 's' &&
       tokens[22].escapedCharType == SEQUENCE && tokens[23].character == '*' &&
       tokens[23].tokenType == STAR && tokens[24].character == '[' &&
       tokens[24].tokenType == OPEN_BRACKET && tokens[25].character == '.' &&
       tokens[25].tokenType == DOT && tokens[25].insideCharacterClass &&
       tokens[26].character == '-' && tokens[26].tokenType == LITERAL &&
       tokens[26].insideCharacterClass && tokens[27].character == ']' &&
       tokens[27].tokenType == CLOSE_BRACKET && tokens[28].character == '?' &&
       tokens[28].tokenType == QUESTION && tokens[29].character == '\\' &&
       tokens[29].tokenType == BACKLASH && tokens[29].escapedChar == 's' &&
       tokens[29].escapedCharType == SEQUENCE && tokens[30].character == '*' &&
       tokens[30].tokenType == STAR && tokens[31].character == '(' &&
       tokens[31].tokenType == OPEN_P && tokens[32].character == '\\' &&
       tokens[32].tokenType == BACKLASH && tokens[32].escapedChar == 'd' &&
       tokens[32].escapedCharType == SEQUENCE && tokens[33].character == '{' &&
       tokens[33].tokenType == LITERAL && tokens[34].character == '4' &&
       tokens[34].tokenType == LITERAL && tokens[35].character == '}' &&
       tokens[35].tokenType == LITERAL && tokens[36].character == ')' &&
       tokens[36].tokenType == CLOSE_P && tokens[37].character == '$' &&
       tokens[37].tokenType == LITERAL);

  if (ok != 1) {
    printf(RED "test_mixed_contexts failed:\n" RESET);
    printf("Token count: %d (expected 38)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 38; i++) {
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

int test_unmatched_brackets() {
  const char *regex = "[abc";
  // Missing closing ], tokenized as-is
  token *tokens = malloc(4 * sizeof(token)); // 4 tokens: [, a, b, c
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 4 && tokens[0].character == '[' &&
            tokens[0].tokenType == OPEN_BRACKET && tokens[1].character == 'a' &&
            tokens[1].tokenType == LITERAL && tokens[1].insideCharacterClass &&
            tokens[2].character == 'b' && tokens[2].tokenType == LITERAL &&
            tokens[2].insideCharacterClass && tokens[3].character == 'c' &&
            tokens[3].tokenType == LITERAL && tokens[3].insideCharacterClass);

  if (ok != 1) {
    printf(RED "test_unmatched_brackets failed:\n" RESET);
    printf("Token count: %d (expected 4)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 4; i++) {
      printf("token %d, character: %c, type: %s, insideCharacterClass: %s\n",
             i + 1, tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? "true" : "false");
    }
  }

  free(tokens);
  return ok;
}

int test_unmatched_parens() {
  const char *regex = "(abc))";
  // Extra closing ), tokenized as-is
  token *tokens = malloc(6 * sizeof(token)); // 6 tokens: (, a, b, c, ), )
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 6 && tokens[0].character == '(' &&
            tokens[0].tokenType == OPEN_P && tokens[1].character == 'a' &&
            tokens[1].tokenType == LITERAL && tokens[2].character == 'b' &&
            tokens[2].tokenType == LITERAL && tokens[3].character == 'c' &&
            tokens[3].tokenType == LITERAL && tokens[4].character == ')' &&
            tokens[4].tokenType == CLOSE_P && tokens[5].character == ')' &&
            tokens[5].tokenType == CLOSE_P);

  if (ok != 1) {
    printf(RED "test_unmatched_parens failed:\n" RESET);
    printf("Token count: %d (expected 6)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 6; i++) {
      printf("token %d, character: %c, type: %s\n", i + 1, tokens[i].character,
             tokenNames[tokens[i].tokenType]);
    }
  }

  free(tokens);
  return ok;
}

int test_char_class_state() {
  const char *regex = "a[b]c[d]e";
  // Verify insideCharacterClass toggles correctly
  token *tokens =
      malloc(9 * sizeof(token)); // 9 tokens: a, [, b, ], c, [, d, ], e
  int tokenCount = tokeniser(regex, tokens);
  int ok =
      (tokenCount == 9 && tokens[0].character == 'a' &&
       tokens[0].tokenType == LITERAL && !tokens[0].insideCharacterClass &&
       tokens[1].character == '[' && tokens[1].tokenType == OPEN_BRACKET &&
       !tokens[1].insideCharacterClass && tokens[2].character == 'b' &&
       tokens[2].tokenType == LITERAL && tokens[2].insideCharacterClass &&
       tokens[3].character == ']' && tokens[3].tokenType == CLOSE_BRACKET &&
       !tokens[3].insideCharacterClass && tokens[4].character == 'c' &&
       tokens[4].tokenType == LITERAL && !tokens[4].insideCharacterClass &&
       tokens[5].character == '[' && tokens[5].tokenType == OPEN_BRACKET &&
       !tokens[5].insideCharacterClass && tokens[6].character == 'd' &&
       tokens[6].tokenType == LITERAL && tokens[6].insideCharacterClass &&
       tokens[7].character == ']' && tokens[7].tokenType == CLOSE_BRACKET &&
       !tokens[7].insideCharacterClass && tokens[8].character == 'e' &&
       tokens[8].tokenType == LITERAL && !tokens[8].insideCharacterClass);

  if (ok != 1) {
    printf(RED "test_char_class_state failed:\n" RESET);
    printf("Token count: %d (expected 9)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 9; i++) {
      printf("token %d, character: %c, type: %s, insideCharacterClass: %s\n",
             i + 1, tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].insideCharacterClass ? "true" : "false");
    }
  }

  free(tokens);
  return ok;
}

int test_token_positions() {
  const char *regex = "a\\nbc";
  // Check that positions are correct, especially after escapes
  token *tokens = malloc(4 * sizeof(token)); // 4 tokens: a, \n, b, c
  int tokenCount = tokeniser(regex, tokens);
  int ok = (tokenCount == 4 && tokens[0].character == 'a' &&
            tokens[0].tokenType == LITERAL && tokens[0].position == 0 &&
            tokens[1].character == '\\' && tokens[1].tokenType == BACKLASH &&
            tokens[1].escapedChar == 'n' &&
            tokens[1].escapedCharType == SEQUENCE && tokens[1].position == 1 &&
            tokens[2].character == 'b' && tokens[2].tokenType == LITERAL &&
            tokens[2].position == 3 && tokens[3].character == 'c' &&
            tokens[3].tokenType == LITERAL && tokens[3].position == 4);

  if (ok != 1) {
    printf(RED "test_token_positions failed:\n" RESET);
    printf("Token count: %d (expected 4)\n", tokenCount);
    for (int i = 0; i < tokenCount && i < 4; i++) {
      printf("token %d, character: %c, type: %s, position: %d\n", i + 1,
             tokens[i].character, tokenNames[tokens[i].tokenType],
             tokens[i].position);
      if (tokens[i].tokenType == BACKLASH) {
        printf("  escapedChar: %c, escapedCharType: %s\n",
               tokens[i].escapedChar, tokenNames[tokens[i].escapedCharType]);
      }
    }
  }
  free(tokens);
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
  } tests[] = {{"test_empty_regex", test_empty_regex},
               {"test_single_char", test_single_char},
               {"test_single_special", test_single_special},
               {"test_start_anchor", test_start_anchor},
               {"test_parser_basic", test_parser_basic},
               {"test_char_class_basic", test_char_class_basic},
               {"test_char_class_special_chars", test_char_class_special_chars},
               {"test_char_class_negation", test_char_class_negation},
               {"test_char_class_escaped", test_char_class_escaped},
               {"test_escape_special", test_escape_special},
               {"test_escape_literal", test_escape_literal},
               {"test_escape_backslash", test_escape_backslash},
               {"test_escape_end_of_string", test_escape_end_of_string},
               {"test_quantifiers", test_quantifiers},
               {"test_quantifier_on_group", test_quantifier_on_group},
               {"test_consecutive_quantifiers", test_consecutive_quantifiers},
               {"test_nested_groups", test_nested_groups},
               {"test_alternation", test_alternation},
               {"test_empty_alternation", test_empty_alternation},
               {"test_email_pattern", test_email_pattern},
               {"test_mixed_contexts", test_mixed_contexts},
               {"test_unmatched_brackets", test_unmatched_brackets},
               {"test_unmatched_parens", test_unmatched_parens},
               {"test_token_positions", test_token_positions},
               {"test_char_class_state", test_char_class_state}};

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
