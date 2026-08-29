CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g -I src
LDFLAGS = -lm

SRC_DIR = src
BIN_DIR = bin
TEST_DIR = test

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = $(BIN_DIR)/interpreter

.PHONY: all clean test commit run

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(BIN_DIR)

test: $(TARGET)
	@echo "Running tests..."
	./$(TARGET) $(TEST_DIR)/test.lang

run: $(TARGET)
	./$(TARGET)

commit: all
	git add -A
	git commit -m "Update: auto-commit" || true
	git push || echo "No remote configured"
