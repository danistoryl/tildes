#ifndef INTERPRETER_CLI_H
#define INTERPRETER_CLI_H

#include <stdbool.h>

// --- CLI Configuration ---
typedef struct {
    bool repl_mode;
    bool verbose;
    bool debug_ast;
    bool debug_bytecode;
    const char* filename;
    int argc;
    char** argv;
} CLIConfig;

// CLI functions
CLIConfig cli_parse(int argc, char** argv);
void cli_print_help(const char* program_name);
void cli_print_version(void);
bool cli_run_repl(void);
int cli_run_file(CLIConfig* config);

#endif
