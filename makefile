# Set compiler
CC = gcc

# Set name for the executable
NAME = hawk

# Directories
BUILD = build
SRC = src

# Flags for compiling
DEBUG_CFLAGS = -Wall -g
RELEASE_CFLAGS = -O2
LDFLAGS = -lpcap -pthread -lcap

# Get all the source files in the SRC directory and its subdirectories
SRCFILES = $(shell find $(SRC) -name '*.c')

# Generate object file names from source file names
OBJFILES = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SRCFILES))

.PHONY: debug release clean

# Target to build the executable with debug flags
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(OBJFILES)
		@echo "Building $(NAME) in debug mode"
		@$(CC) $(CFLAGS) $^ -o $(NAME) $(LDFLAGS)
		@echo "Done!"


# Target to build the executable with release flags
release: CFLAGS = $(RELEASE_CFLAGS)
release: $(OBJFILES)
	@echo "Building $(NAME) in release mode"
	@$(CC) $(CFLAGS) $^ -o $(NAME) $(LDFLAGS)
	@echo "Done!"


# Rule to compile each source file into an object file
$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Target to clean up generated file
clean:
	@rm -rf $(OBJFILES) $(NAME)
