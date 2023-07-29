IDIR = include
SRCDIR = src
ODIR = obj
BDIR = bin

CC = gcc
CFLAGS = -I $(IDIR) -Wall -g

LIB =
DEF = -DDEBUG_FLAG -DEXPERIMENTAL=0
BIN = recursive-descent-parser
BINPATH = $(BDIR)/$(BIN)

# List of source files (located in the src/ directory)
SRCS = $(wildcard $(SRCDIR)/*.c)

# Exclude the main program file from the list of source files
# SRCS := $(filter-out $(SRCDIR)/recursive-descent-parser.c, $(SRCS))

# Generate object file names with the "obj/" prefix
OBJS = $(patsubst $(SRCDIR)/%.c, $(ODIR)/%.o, $(SRCS))

# Rule to create the "obj" directory if it doesn't exist
$(shell mkdir -p $(ODIR))

# Rule to create the "out" directory if it doesn't exist
$(shell mkdir -p $(BDIR))

# Rule to compile each module into its object file in the "obj/" directory
$(ODIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(DEF) $< -o $@

# Rule to compile the executable program
$(BINPATH): $(OBJS)
	$(CC) $(CFLAGS) $(DEF) $(LIB) $(OBJS) -o $(BINPATH)

.PHONY: clean

clean:
	rm -rf *~ $(ODIR) $(BDIR)

depend:
	makedepend -Y -- $(CFLAGS) $(DEFS) -- $(SRCDIR)/*.c

# DO NOT DELETE

src/arguments.o: include/arguments.h
src/grammar.o: include/grammar.h include/node.h include/linked-list.h
src/grammar.o: include/helpers.h
src/helpers.o: include/helpers.h
src/lexer.o: include/lexer.h include/token.h include/helpers.h
src/linked-list.o: include/linked-list.h
src/node.o: include/node.h include/linked-list.h include/helpers.h
src/parser.o: include/parser.h include/linked-list.h include/arguments.h
src/parser.o: include/node.h include/helpers.h include/grammar.h
src/parser.o: include/lexer.h include/token.h
src/recursive-descent-parser.o: include/parser.h include/linked-list.h
src/recursive-descent-parser.o: include/arguments.h include/node.h
src/recursive-descent-parser.o: include/helpers.h include/grammar.h
src/recursive-descent-parser.o: include/lexer.h include/token.h
