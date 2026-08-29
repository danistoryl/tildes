CC = gcc
CFLAGS = -shared -fPIC -g
LDFLAGS = -lm

LIB_NAME = libadvanced.so
TEST_FILE = test_advanced.lang
INTERPRETER = ../../bin/interpreter

all: $(LIB_NAME) run

$(LIB_NAME): c_lib_advanced.c
	@echo "Compiling advanced C library..."
	$(CC) $(CFLAGS) -o $(LIB_NAME) c_lib_advanced.c $(LDFLAGS)
	@echo "Compiled: $(LIB_NAME)"

run: $(LIB_NAME)
	@echo ""
	@echo "Running advanced FFI tests..."
	@$(INTERPRETER) $(TEST_FILE)

clean:
	rm -f $(LIB_NAME) *.o

.PHONY: all run clean
