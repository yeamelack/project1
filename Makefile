# Makefile

# Variables
CC=gcc
CFLAGS=-Wall -g
N ?= 1
SOURCE_FILE=template.c
TEST_DIR=./test
BINARIES=$(addprefix $(TEST_DIR)/test_, $(shell seq 1 $(N)))

# Default target
all: autograder $(BINARIES)

# Compile autograder
autograder: autograder.c
	$(CC) $(CFLAGS) -o $@ $^

# Compile template.c into N binaries
$(TEST_DIR)/test_%: $(SOURCE_FILE)
	mkdir -p $(TEST_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Clean the build
clean:
	rm -f autograder $(BINARIES)

.PHONY: all clean
