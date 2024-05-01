CC=gcc
CFLAGS=-g -Wall
LIBDIR=lib
OBJDIR=obj
LIBS=$(wildcard $(LIBDIR)/*.c)
OBJECTS=$(patsubst $(LIBS)/%.c, $(OBJDIR)/%.o, $(LIBS))
BIN=fib

all: $(BIN)

run: $(BIN)
	./$(BIN)

release: CFLAGS=-Wall -O2 -DNDEBUG
release: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) main.c -o $@

$(OBJDIR)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BIN) $(OBJDIR)/*.o


