# Set compiler
CC = gcc

# Set name for the executable
NAME = hawk

# Directories
BUILD = build
SRC = src

# Files
VERSION_FILE = ./data/version/version.json
VERSION_HEADER_FILE = ./include/version.h
VERSION = $(shell jq .version $(VERSION_FILE))

# ------------------------------------------------------
# 					Code compilation
# ------------------------------------------------------

# Flags for compiling
DEBUG_CFLAGS = -Wall -g
RELEASE_CFLAGS =
LDFLAGS = -lpcap -pthread -lcjson

# Get all the source files in the SRC directory and its subdirectories
SRCFILES = $(shell find $(SRC) -name '*.c')

# Generate object file names from source file names
OBJFILES = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SRCFILES))

.PHONY: debug release clean setup-version

# Target to build the executable with debug flags
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(OBJFILES)
	@$(MAKE) setup-version --no-print-directory
	@echo "Building $(NAME) in debug mode"
	@$(CC) $(CFLAGS) $^ -o $(NAME) $(LDFLAGS)
	@echo "Done!"


# Target to build the executable with release flags
build_release: CFLAGS = $(RELEASE_CFLAGS)
build_release: $(OBJFILES)
	@echo "Building $(NAME) in release mode"
	@$(CC) $(CFLAGS) $^ -o $(NAME) $(LDFLAGS)
	@echo "Done!"

# To clean out debug code
release:
	@$(MAKE) setup-version --no-print-directory
	@$(MAKE) clean --no-print-directory
	@$(MAKE) build_release --no-print-directory


# Rule to compile each source file into an object file
$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Target to clean up generated file
clean:
	@rm -rf $(OBJFILES) $(NAME)

setup-version:
	@echo "Setting up version"
	@echo "#pragma once" > $(VERSION_HEADER_FILE)
	@echo '#define VERSION $(VERSION)' >> $(VERSION_HEADER_FILE)

setup-hooks:
	@echo "Setting up git hooks"
	cp ./data/hooks/* ./.git/hooks/
	chmod +x ./.git/hooks/*

test-fedora:
	act --rm -W ./.github/workflows/fedora-build.yml

test-ubuntu:
	act --rm -W ./.github/workflows/ubuntu-build.yml
