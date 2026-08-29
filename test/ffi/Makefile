# Test Makefile for FFI, CLI args, and bytecode compilation

CC = gcc
CFLAGS = -shared -fPIC
INTERPRETER = ../bin/interpreter

.PHONY: all clean test

all: test

test: libtest.so
	@echo "Running FFI test..."
	@if [ -f $(INTERPRETER) ]; then \
		$(INTERPRETER) test_ffi.lang && echo "OK: test_ffi.lang" || echo "FAIL: test_ffi.lang"; \
	else \
		echo "Interpreter not found. Run 'make' in parent directory first."; \
	fi
	
	@echo ""
	@echo "Running CLI args test..."
	@if [ -f $(INTERPRETER) ]; then \
		$(INTERPRETER) test_cli_args.lang hello world && echo "OK: test_cli_args.lang" || echo "FAIL: test_cli_args.lang"; \
	else \
		echo "Interpreter not found."; \
	fi
	
	@echo ""
	@echo "Running compile test..."
	@if [ -f $(INTERPRETER) ]; then \
		$(INTERPRETER) test_compile.lang && echo "OK: test_compile.lang" || echo "FAIL: test_compile.lang"; \
	else \
		echo "Interpreter not found."; \
	fi
	
	@echo ""
	@echo "================================="
	@echo "FFI tests completed!"
	@echo "================================="

libtest.so: test_lib.c
	@echo "Compiling C library for FFI test..."
	$(CC) $(CFLAGS) -o libtest.so test_lib.c

clean:
	rm -f *.so *.fbc
