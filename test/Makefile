# Test Makefile for FCL Language Interpreter

INTERPRETER = ../bin/interpreter
TEST_FILES = test_variables.lang test_control.lang test_loops.lang test_functions.lang test_structs.lang test_enum.lang test_modules.lang test_builtins.lang test_types.lang
PASSED = 0
FAILED = 0

.PHONY: all clean test

all: test

test: $(TEST_FILES)
	@echo "========================================="
	@echo "Running FCL Language Test Suite"
	@echo "========================================="
	@for test in $(TEST_FILES); do \
		echo "Testing $$test..."; \
		if $(INTERPRETER) $$test; then \
			echo "✅ PASS: $$test"; \
			PASSED=$$((PASSED + 1)); \
		else \
			echo "❌ FAIL: $$test"; \
			FAILED=$$((FAILED + 1)); \
		fi; \
		echo ""; \
	done
	@echo "========================================="
	@echo "Test Summary"
	@echo "========================================="
	@echo "Tests completed!"

clean:
	rm -f *.out

# Individual test targets
test_variables:
	@echo "Testing variables..."
	$(INTERPRETER) test_variables.lang

test_control:
	@echo "Testing control flow..."
	$(INTERPRETER) test_control.lang

test_loops:
	@echo "Testing loops..."
	$(INTERPRETER) test_loops.lang

test_functions:
	@echo "Testing functions..."
	$(INTERPRETER) test_functions.lang

test_structs:
	@echo "Testing structs..."
	$(INTERPRETER) test_structs.lang

test_enum:
	@echo "Testing enums..."
	$(INTERPRETER) test_enum.lang

test_modules:
	@echo "Testing modules..."
	$(INTERPRETER) test_modules.lang

test_builtins:
	@echo "Testing built-ins..."
	$(INTERPRETER) test_builtins.lang

test_types:
	@echo "Testing types..."
	$(INTERPRETER) test_types.lang
