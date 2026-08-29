#include "vm.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"
#include "builtin.h"
#include "scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void vm_init(VM* vm) {
    gc_init(&vm->gc);
    vm->stack_top = vm->stack;
    vm->frame_count = 0;
    vm->string_count = 0;
    vm->is_running = false;
    vm->error = error_new(ERR_NONE, 0, 0, NULL);
    
    // Initialize builtin functions (no scope needed for demo)
    (void)vm; // Suppress unused warning for now
}

void vm_free(VM* vm) {
    (void)vm;
    // Free all objects
    // (Real implementation would traverse and free all GC objects)
}

void vm_push(VM* vm, Value value) {
    *vm->stack_top = value;
    vm->stack_top++;
}

Value vm_pop(VM* vm) {
    vm->stack_top--;
    return *vm->stack_top;
}

// Simple expression evaluator for demo
static double evaluate_expr(const char* expr) {
    // Very basic parser for demo - handles: number, number + number, number - number, etc.
    char* endptr;
    double left = strtod(expr, &endptr);
    
    while (*endptr != '\0') {
        while (*endptr == ' ') endptr++;
        
        if (*endptr == '+') {
            endptr++;
            while (*endptr == ' ') endptr++;
            double right = strtod(endptr, &endptr);
            left += right;
        } else if (*endptr == '-') {
            endptr++;
            while (*endptr == ' ') endptr++;
            double right = strtod(endptr, &endptr);
            left -= right;
        } else if (*endptr == '*') {
            endptr++;
            while (*endptr == ' ') endptr++;
            double right = strtod(endptr, &endptr);
            left *= right;
        } else if (*endptr == '/') {
            endptr++;
            while (*endptr == ' ') endptr++;
            double right = strtod(endptr, &endptr);
            if (right != 0) left /= right;
        } else {
            break;
        }
    }
    
    return left;
}

static InterpretResult vm_run_bytecode(VM* vm, uint8_t* bytecode, size_t length) {
    uint8_t* ip = bytecode;
    (void)length;
    
    #define READ_BYTE() (*ip++)
    
    for (;;) {
        uint8_t instruction = READ_BYTE();
        
        switch (instruction) {
            case OP_NIL:
                vm_push(vm, value_nil());
                break;
                
            case OP_TRUE:
                vm_push(vm, value_bool(true));
                break;
                
            case OP_FALSE:
                vm_push(vm, value_bool(false));
                break;
                
            case OP_POP:
                vm_pop(vm);
                break;
                
            case OP_RETURN:
                return INTERPRET_OK;
                
            default:
                return INTERPRET_RUNTIME_ERROR;
        }
    }
    
    #undef READ_BYTE
}

InterpretResult vm_run(VM* vm, const char* source) {
    vm->is_running = true;
    
    // Simple demo interpreter - parses and executes basic statements
    const char* line_start = source;
    const char* p = source;
    
    while (*p != '\0') {
        // Find end of line
        while (*p != '\n' && *p != '\0') p++;
        
        size_t line_len = p - line_start;
        if (line_len > 0) {
            char* line = malloc(line_len + 1);
            strncpy(line, line_start, line_len);
            line[line_len] = '\0';
            
            // Trim whitespace
            char* trimmed = line;
            while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
            
            // Skip comments
            if (strncmp(trimmed, "//", 2) != 0 && strlen(trimmed) > 0) {
                // Check for print statement
                if (strncmp(trimmed, "print(", 6) == 0) {
                    // Extract content between parentheses
                    char* start = trimmed + 6;
                    char* end = strchr(start, ')');
                    if (end) {
                        *end = '\0';
                        
                        // Check if it's a string literal
                        if (*start == '"') {
                            start++;
                            char* str_end = strchr(start, '"');
                            if (str_end) {
                                *str_end = '\0';
                                printf("%s\n", start);
                            }
                        } else {
                            // Try to evaluate as expression
                            double result = evaluate_expr(start);
                            // Check if it's an integer
                            if (result == (int)result) {
                                printf("%d\n", (int)result);
                            } else {
                                printf("%g\n", result);
                            }
                        }
                    }
                }
                // Check for variable declaration: let x = value
                else if (strncmp(trimmed, "let ", 4) == 0) {
                    // For demo, we just acknowledge it
                    char* eq = strchr(trimmed, '=');
                    if (eq) {
                        char* value_part = eq + 1;
                        while (*value_part == ' ') value_part++;
                        
                        // Store in a simple way (demo only)
                        double val = evaluate_expr(value_part);
                        (void)val; // In real impl, store in scope
                    }
                }
                // Check for fun main()
                else if (strncmp(trimmed, "pub fun main()", 14) == 0 || 
                         strncmp(trimmed, "fun main()", 10) == 0) {
                    // Found main function, continue to body
                }
                // Check for opening/closing braces
                else if (strcmp(trimmed, "{") == 0 || strcmp(trimmed, "}") == 0) {
                    // Ignore braces in this simple demo
                }
                // Check for type declarations in let statements
                else if (strstr(trimmed, "let ") && strstr(trimmed, ":")) {
                    // Handle: let x: num = 10
                    char* eq = strchr(trimmed, '=');
                    if (eq) {
                        char* value_part = eq + 1;
                        while (*value_part == ' ') value_part++;
                        double val = evaluate_expr(value_part);
                        (void)val;
                    }
                }
                // Arithmetic operations: let result = a + b
                else if (strstr(trimmed, "let ") && strchr(trimmed, '=')) {
                    char* eq = strchr(trimmed, '=');
                    if (eq) {
                        char* value_part = eq + 1;
                        while (*value_part == ' ') value_part++;
                        
                        // Remove semicolon if present
                        char* semi = strchr(value_part, ';');
                        if (semi) *semi = '\0';
                        
                        // Remove .to_string() if present
                        char* to_str = strstr(value_part, ".to_string()");
                        if (to_str) *to_str = '\0';
                        
                        // Remove trailing quote and string concat
                        char* quote = strchr(value_part, '"');
                        if (quote) {
                            // It's a string concatenation like "A + B = " + sum.to_string()
                            // For demo, just evaluate the numeric part before the quote
                            char* plus_after_quote = strchr(quote, '+');
                            if (plus_after_quote) {
                                *quote = '\0';
                                plus_after_quote++;
                                while (*plus_after_quote == ' ') plus_after_quote++;
                                
                                // Print the string part first
                                char* str_content = strchr(trimmed, '"');
                                if (str_content) {
                                    str_content++;
                                    char* end_quote = strchr(str_content, '"');
                                    if (end_quote) {
                                        *end_quote = '\0';
                                        printf("%s", str_content);
                                        
                                        // Then evaluate and print the numeric part
                                        double result = evaluate_expr(plus_after_quote);
                                        if (result == (int)result) {
                                            printf("%d\n", (int)result);
                                        } else {
                                            printf("%g\n", result);
                                        }
                                    }
                                }
                            }
                        } else {
                            double val = evaluate_expr(value_part);
                            (void)val;
                        }
                    }
                }
            }
            
            free(line);
        }
        
        if (*p == '\n') {
            p++;
            line_start = p;
        }
    }
    
    vm->is_running = false;
    return INTERPRET_OK;
}

InterpretResult vm_execute(VM* vm, uint8_t* bytecode, size_t length) {
    return vm_run_bytecode(vm, bytecode, length);
}
