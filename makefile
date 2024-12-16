CC = gcc

NAME = hawk

BUILD = build
SRC = src

CFLAGS = -Wall -g
LDFLAGS = -lpcap -pthread -lcap

# Get all the source files in the SRC directory and its subdirectories
SRCFILES = $(shell find $(SRC) -name '*.c')

# Generate object file names from source file names
OBJFILES = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SRCFILES))


.PHONY: release clean

# Target to build the executable
release: $(OBJFILES)
	@echo "Building $(NAME)"
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
