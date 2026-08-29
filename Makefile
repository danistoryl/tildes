# Makefile for interpreter - Modern GCC

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g -O2
LDFLAGS = 

SRCS = main.c lexer.c token.c value.c
OBJS = $(SRCS:.c=.o)
TARGET = interpreter

.PHONY: all clean test commit

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET) test.lang

commit: clean
	git add -A
	git commit -m "Update interpreter code"
	git push origin HEAD
