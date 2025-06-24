# === Compiler & Flags ===
CC = gcc
CFLAGS = -Iinclude -Ilib/bigint/include -Wall -Wextra -g

# === Paths ===
SRC_DIR = src
BIGINT_SRC_DIR = lib/bigint/src
OBJ_DIR = build
BIN_DIR = bin

# === Sources ===
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/rsa.c $(SRC_DIR)/cli.c $(BIGINT_SRC_DIR)/bigint.c
OBJ_FILES = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# === Target Binary ===
TARGET = $(BIN_DIR)/rsa-simple.out

# === Build Rules ===
all: $(TARGET)

# Create binary directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Create object directory
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link all objects
$(TARGET): $(BIN_DIR) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean

