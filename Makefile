CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Ilib/bigint/include -g

# Source files
RSA_SRC = src/rsa.c
CLI_SRC = src/cli.c
BIGINT_SRC = lib/bigint/src/bigint.c
TEST_SRC = src/test_rsa.c

# Object files
RSA_OBJ = build/rsa.o
BIGINT_OBJ = build/bigint.o
TEST_OBJ = build/test_rsa.o
CLI_OBJ = build/cli.o

# Output binary
TEST_BIN = test_rsa

# Default target
all: $(TEST_BIN)

# Compile each file explicitly
$(RSA_OBJ): $(RSA_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIGINT_OBJ): $(BIGINT_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): $(TEST_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLI_OBJ): $(CLI_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything
$(TEST_BIN): $(TEST_OBJ) $(RSA_OBJ) $(BIGINT_OBJ) $(CLI_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Run tests
test: $(TEST_BIN)
	./$(TEST_BIN)

# Clean build artifacts
clean:
	rm -rf build $(TEST_BIN)

.PHONY: all test clean

