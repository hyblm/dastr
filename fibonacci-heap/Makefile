CC=gcc
CFLAGS=-g -Wall
LIBDIR=lib
OBJDIR=obj
LIBS=$(wildcard $(LIBDIR)/*.c)
OBJECTS=$(patsubst $(LIBS)/%.c, $(OBJDIR)/%.o, $(LIBS))
BIN=fib

all: $(BIN)

run: $(BIN)
	./fib

release: CFLAGS=-Wall -O2 -DNDEBUG
release: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) main.c -o $@

$(OBJ)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) fib obj/*.o


