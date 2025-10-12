# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -IRegexEngine/Parser
LDFLAGS = 

# Directories
SRC_DIR = RegexEngine/Parser
TEST_DIR = tests/RegexEngine/Parser
BUILD_DIR = build
BIN_DIR = bin

# Source files
PARSER_SRC = $(SRC_DIR)/Parser.c
PARSER_OBJ = $(BUILD_DIR)/Parser.o

# Test sources and binaries
TEST_TOKENISER_SRC = $(TEST_DIR)/test_tokeniser.c
TEST_TOKENISER_BIN = $(BIN_DIR)/test_tokeniser

TEST_VALID_REGEX_SRC = $(TEST_DIR)/test_valid_regex.c
TEST_VALID_REGEX_BIN = $(BIN_DIR)/test_valid_regex

# All test binaries
TEST_BINS = $(TEST_TOKENISER_BIN) $(TEST_VALID_REGEX_BIN)

# Default target
all: $(TEST_BINS)

# Create directories
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# Compile parser object (shared between tests)
$(PARSER_OBJ): $(PARSER_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build test executables
$(TEST_TOKENISER_BIN): $(TEST_TOKENISER_SRC) $(PARSER_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_VALID_REGEX_BIN): $(TEST_VALID_REGEX_SRC) $(PARSER_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Run all tests
test: $(TEST_BINS)
	@echo "Running all tests..."
	@for test in $(TEST_BINS); do \
		echo "Running $$test..."; \
		$$test || exit 1; \
	done

# Run individual tests
test_tokeniser: $(TEST_TOKENISER_BIN)
	$<

test_valid_regex: $(TEST_VALID_REGEX_BIN)
	$<

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all             - Build all test executables"
	@echo "  test            - Run all tests"
	@echo "  test_tokeniser  - Run tokeniser test only"
	@echo "  test_valid_regex - Run valid regex test only"
	@echo "  clean           - Remove all build artifacts"
	@echo "  help            - Show this help message"

.PHONY: all test test_tokeniser test_valid_regex clean help
