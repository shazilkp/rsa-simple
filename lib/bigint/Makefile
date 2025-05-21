# === Compiler and Flags ===
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

# === Files and Paths ===
SRC = src/bigint.c
TEST = test/libtest.c
OUT = libtest

# === Default Target ===
all: $(OUT)

$(OUT): $(SRC) $(TEST)
	$(CC) $(CFLAGS) $(SRC) $(TEST) -o $(OUT)

# === Clean Target ===
clean:
	rm -f $(OUT)

.PHONY: all clean

