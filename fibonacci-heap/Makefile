C=gcc
CFALGS=-Wall
LIBDIR=lib
OBJDIR=obj
LIBS=$(wildcard $(LIBDIR)/*.c)
OBJECTS=$(patsubst $(LIBS)/%.c, $(OBJDIR)/%.o, $(LIBS))
BIN=fib

all: $(BIN)

release: CFLAGS=-Wall -O2 -DNDEBUG
release: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) main.c -o $@

$(OBJ)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) fib obj/*.o
