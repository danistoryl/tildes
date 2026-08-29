#include "cli.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cli_print_help(const char* program_name) {
    printf("Usage: %s [options] [script]\n", program_name);
    printf("\nOptions:\n");
    printf("  -r, --repl       Run in REPL mode\n");
    printf("  -v, --verbose    Enable verbose output\n");
    printf("  -d, --debug      Debug AST and bytecode\n");
    printf("  -h, --help       Show this help message\n");
    printf("  --version        Show version information\n");
}

void cli_print_version(void) {
    printf("FCL Language Interpreter v0.1.0\n");
    printf("Built with modern C11 and GCC\n");
}

CLIConfig cli_parse(int argc, char** argv) {
    CLIConfig config = {
        .repl_mode = false,
        .verbose = false,
        .debug_ast = false,
        .debug_bytecode = false,
        .filename = NULL,
        .argc = argc,
        .argv = argv
    };
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--repl") == 0) {
            config.repl_mode = true;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            config.verbose = true;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
            config.debug_ast = true;
            config.debug_bytecode = true;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            cli_print_help(argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "--version") == 0) {
            cli_print_version();
            exit(0);
        } else if (argv[i][0] != '-') {
            config.filename = argv[i];
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            cli_print_help(argv[0]);
            exit(1);
        }
    }
    
    return config;
}

bool cli_run_repl(void) {
    char line[1024];
    VM vm;
    vm_init(&vm);
    
    printf("FCL Language v0.1.0 - Type 'exit' to quit\n");
    
    for (;;) {
        printf("> ");
        
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strcmp(line, "exit") == 0) {
            break;
        }
        
        InterpretResult result = vm_run(&vm, line);
        if (result == INTERPRET_RUNTIME_ERROR) {
            fprintf(stderr, "Runtime error occurred\n");
        }
    }
    
    vm_free(&vm);
    return true;
}

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Could not open file \"%s\"\n", path);
        return NULL;
    }
    
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fprintf(stderr, "Not enough memory to read file\n");
        fclose(file);
        return NULL;
    }
    
    size_t read = fread(buffer, 1, size, file);
    if (read < size) {
        fprintf(stderr, "Could not read file completely\n");
        free(buffer);
        fclose(file);
        return NULL;
    }
    
    buffer[read] = '\0';
    fclose(file);
    return buffer;
}

int cli_run_file(CLIConfig* config) {
    if (!config->filename) {
        return cli_run_repl() ? 0 : 1;
    }
    
    char* source = read_file(config->filename);
    if (!source) {
        return 1;
    }
    
    VM vm;
    vm_init(&vm);
    
    if (config->verbose) {
        printf("Running %s...\n", config->filename);
    }
    
    InterpretResult result = vm_run(&vm, source);
    
    vm_free(&vm);
    free(source);
    
    return (result == INTERPRET_OK) ? 0 : 1;
}
