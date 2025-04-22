# Set compiler
CC = gcc

# Set name for the executable
NAME = hawk

# Directories
PACKAGES = packages
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
LDFLAGS = -lpcap --pthread -lcjson -lm
# lm is used by argtable3


# Get all the source files in the SRC directory and its subdirectories
SRCFILES = $(shell find $(SRC) -name '*.c')
PKG_SRCFILES = $(shell find $(PACKAGES) -name '*.c')

# Generate object file names from source file names
OBJFILES = $(patsubst $(SRC)/%.c, $(BUILD)/$(SRC)/%.o, $(SRCFILES))
PKG_OBJFILES = $(patsubst $(PACKAGES)/%.c, $(BUILD)/$(PACKAGES)/%.o, $(PKG_SRCFILES))

.PHONY: debug release clean setup-version

# Target to build the executable with debug flags
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(OBJFILES) $(PKG_OBJFILES)
	@$(MAKE) setup-version --no-print-directory
	@echo "Building $(NAME) in debug mode"
	@$(CC) $(CFLAGS) $^ -o $(NAME) $(LDFLAGS)
	@echo "Done!"


# Target to build the executable with release flags
build_release: CFLAGS = $(RELEASE_CFLAGS)
build_release: $(OBJFILES) $(PKG_OBJFILES)
	@echo "Building $(NAME) in release mode"
	@$(CC) $(CFLAGS) $^ -o $(NAME) $(LDFLAGS)
	@echo "Done!"

# To clean out debug code
release:
	@$(MAKE) setup-version --no-print-directory
	@$(MAKE) clean --no-print-directory
	@$(MAKE) build_release --no-print-directory


# Rule to compile each source file into an object file
# Rule to compile each source file into an object file
$(BUILD)/$(SRC)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/$(PACKAGES)/%.o: $(PACKAGES)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Target to clean up generated file
clean:
	@rm -rf $(BUILD) $(NAME)

setup-version:
	@echo "Setting up version"
	@echo "#pragma once" > $(VERSION_HEADER_FILE)
	@echo '#define VERSION $(VERSION)' >> $(VERSION_HEADER_FILE)

setup-hooks:
	@echo "Setting up git hooks"
	cp ./data/hooks/* ./.git/hooks/
	chmod +x ./.git/hooks/*

test-compile:
	act --rm -W .github/workflows/compile-test.yml
