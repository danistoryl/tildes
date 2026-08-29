# Makefile for interpreter - Modern GCC

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g -O2
LDFLAGS = 

SRCDIR = src
BINDIR = bin
TESTDIR = test

SRCS = $(SRCDIR)/main.c $(SRCDIR)/lexer.c $(SRCDIR)/token.c $(SRCDIR)/value.c
OBJS = $(SRCS:.c=.o)
TARGET = $(BINDIR)/interpreter

.PHONY: all clean test commit dirs

dirs:
	mkdir -p $(BINDIR)

all: dirs $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(SRCDIR) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET) $(TESTDIR)/test.lang

commit: clean
	git add -A
	git commit -m "Refactor: move source to src, binary to bin"
	git push origin HEAD
