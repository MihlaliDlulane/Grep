#include <stdio.h>
#include "Library.h"

void printUsage(void) {
    printf("Usage: -n [String] -E [Regex]\n");
}

int regexSearch(const char *regex, const char *text) {

    for (int i = 0; text[i] != '\0'; i++) {
        if (regex[0] == text[i]) {
            return 1;
        }
    }
    return -1;
}




