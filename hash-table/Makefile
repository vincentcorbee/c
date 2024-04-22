IDIR = include
SDIR = src
BDIR = bin
ODIR = obj

CC = gcc
CFLAGS = -I $(IDIR) -Wall -g
DEF = -DDEBUG_FLAG -DEXPERIMENTAL=0

BIN = hash-table
BINPATH = $(BDIR)/$(BIN)

SRCS = $(wildcard $(SDIR)/*.c)

OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))

$(shell mkdir -p $(ODIR))

$(shell mkdir -p $(BDIR))

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c $(CFLAGS) $(DEF) $< -o $@

$(BINPATH): $(OBJS)
	$(CC) $(CFLAGS) $(DEF) $(LIB) $(OBJS) -o $(BINPATH)

.PHONY: clean

clean:
	rm -rf *~ $(ODIR) $(BDIR)

depend:
	makedepend -Y -- $(CFLAGS) $(DEFS) -- $(SDIR)/*.c

