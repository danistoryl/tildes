CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g -I src
LDFLAGS = -lm

SRC_DIR = src
BIN_DIR = bin
TEST_DIR = test

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = $(BIN_DIR)/interpreter
LDFLAGS += -ldl

.PHONY: all clean test commit run test-ffi

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
	@echo "Running main tests..."
	./$(TARGET) $(TEST_DIR)/test.lang
	@echo ""
	@echo "Running FFI tests..."
	$(MAKE) -C $(TEST_DIR)/ffi

test-ffi: $(TARGET)
	$(MAKE) -C $(TEST_DIR)/ffi

run: $(TARGET)
	./$(TARGET)

commit: all
	git add -A
	git commit -m "feat: add C/C++ FFI, bytecode compiler, CLI args, and FFI test suite" || true
	git push || echo "No remote configured"
