CC = gcc
CFLAGS = -shared -fPIC
INTERPRETER = ../../bin/interpreter

all: test_jit

libbench.so: bench.c
	@echo "Compiling C helper for JIT benchmarks..."
	$(CC) $(CFLAGS) -o libbench.so bench.c
	@echo "✅ C Helper compiled."

test_jit: libbench.so
	@echo "Running JIT Engine Tests..."
	$(INTERPRETER) test_jit.lang

clean:
	rm -f libbench.so *.out

.PHONY: all clean test_jit
