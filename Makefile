# Compiler and flags
CC=gcc
CFLAGS=-Wall -Wextra -IRegexEngine/Parser

# Source files
PARSER_SRC=RegexEngine/Parser/Parser.C
TEST_SRC=tests/RegexEngine/Parser/test_tokeniser.c

# Output
TEST_BIN=test_tokeniser

# Default target
all: $(TEST_BIN)

# Build test executable (link with parser object)
$(TEST_BIN): $(TEST_SRC) $(PARSER_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) $(PARSER_SRC) -o $(TEST_BIN)

# Run the test
test: $(TEST_BIN)
	./$(TEST_BIN)

# Clean build artifacts
clean:
	rm -f $(TEST_BIN)

.PHONY: all test clean
