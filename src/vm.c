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

// Simple variable storage for demo (replaces scope system for now)
#define MAX_VARIABLES 256
typedef struct {
    char name[64];
    double value;
    bool is_defined;
} Variable;

static Variable variables[MAX_VARIABLES];
static int var_count = 0;

static double get_variable(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (variables[i].is_defined && strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    return 0.0; // Return 0 if not found
}

static void set_variable(const char* name, double value) {
    // Check if variable exists
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            variables[i].is_defined = true;
            return;
        }
    }
    // Add new variable
    if (var_count < MAX_VARIABLES) {
        strncpy(variables[var_count].name, name, 63);
        variables[var_count].name[63] = '\0';
        variables[var_count].value = value;
        variables[var_count].is_defined = true;
        var_count++;
    }
}

void vm_init(VM* vm) {
    gc_init(&vm->gc);
    vm->stack_top = vm->stack;
    vm->frame_count = 0;
    vm->string_count = 0;
    vm->is_running = false;
    vm->error = error_new(ERR_NONE, 0, 0, NULL);
    
    // Initialize builtin functions (no scope needed for demo)
    (void)vm; // Suppress unused warning for now
    
    // Reset variables
    var_count = 0;
    for (int i = 0; i < MAX_VARIABLES; i++) {
        variables[i].is_defined = false;
    }
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

// Helper to extract a number or variable value
static double get_number_or_var(const char* str, char** endptr) {
    while (*str == ' ') str++;
    
    // Check if it starts with a letter (variable)
    if ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || *str == '_') {
        char var_name[64];
        int i = 0;
        while ((*str >= 'a' && *str <= 'z') || 
               (*str >= 'A' && *str <= 'Z') || 
               (*str >= '0' && *str <= '9') || 
               *str == '_') {
            if (i < 63) var_name[i++] = *str;
            str++;
        }
        var_name[i] = '\0';
        *endptr = (char*)str;
        return get_variable(var_name);
    }
    
    // Otherwise parse as number
    return strtod(str, endptr);
}

// Expression evaluator with variable support
static double evaluate_expr(const char* expr) {
    char* endptr;
    double left = get_number_or_var(expr, &endptr);
    
    while (*endptr != '\0') {
        while (*endptr == ' ') endptr++;
        
        if (*endptr == '+') {
            endptr++;
            double right = get_number_or_var(endptr, &endptr);
            left += right;
        } else if (*endptr == '-') {
            endptr++;
            double right = get_number_or_var(endptr, &endptr);
            left -= right;
        } else if (*endptr == '*') {
            endptr++;
            double right = get_number_or_var(endptr, &endptr);
            left *= right;
        } else if (*endptr == '/') {
            endptr++;
            double right = get_number_or_var(endptr, &endptr);
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
                            // Check if it's a variable name first
                            char* var_check = start;
                            while (*var_check == ' ') var_check++;
                            
                            // Simple variable lookup: check if it's just an identifier
                            char* end_ptr = var_check;
                            while ((*end_ptr >= 'a' && *end_ptr <= 'z') || 
                                   (*end_ptr >= 'A' && *end_ptr <= 'Z') || 
                                   (*end_ptr >= '0' && *end_ptr <= '9') || 
                                   *end_ptr == '_') {
                                end_ptr++;
                            }
                            
                            // If it's just an identifier (no operators), try variable lookup
                            if (*end_ptr == '\0' || *end_ptr == ')' || *end_ptr == ' ' || *end_ptr == '\n') {
                                char var_name[64];
                                size_t vlen = end_ptr - var_check;
                                if (vlen > 0 && vlen < 64) {
                                    strncpy(var_name, var_check, vlen);
                                    var_name[vlen] = '\0';
                                    double val = get_variable(var_name);
                                    if ((int)val == val) {
                                        printf("%d\n", (int)val);
                                    } else {
                                        printf("%g\n", val);
                                    }
                                } else {
                                    // Not a valid variable, evaluate as expression
                                    double result = evaluate_expr(start);
                                    if ((int)result == result) {
                                        printf("%d\n", (int)result);
                                    } else {
                                        printf("%g\n", result);
                                    }
                                }
                            } else {
                                // Contains operators, evaluate as expression with variable substitution
                                double result = evaluate_expr(start);
                                if ((int)result == result) {
                                    printf("%d\n", (int)result);
                                } else {
                                    printf("%g\n", result);
                                }
                            }
                        }
                    }
                }
                // Check for variable declaration: let x = value or let x: type = value
                else if (strncmp(trimmed, "let ", 4) == 0) {
                    char* eq = strchr(trimmed, '=');
                    if (eq) {
                        char* value_part = eq + 1;
                        while (*value_part == ' ') value_part++;

                        // Remove semicolon if present
                        char* semi = strchr(value_part, ';');
                        if (semi) *semi = '\0';

                        // Store in a simple way (demo only)
                        double val = evaluate_expr(value_part);

                        // Extract variable name
                        char* name_start = trimmed + 4;
                        while (*name_start == ' ') name_start++;
                        char* name_end = name_start;
                        while (*name_end != ' ' && *name_end != ':' && *name_end != '=') name_end++;

                        char var_name[64];
                        size_t name_len = name_end - name_start;
                        if (name_len >= 64) name_len = 63;
                        strncpy(var_name, name_start, name_len);
                        var_name[name_len] = '\0';

                        set_variable(var_name, val);
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
