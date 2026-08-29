/*
 * main.c - Entry point for the interpreter
 * Modern CLI with REPL and file execution
 */

#include "interpreter.h"
#include "cli.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    CLIConfig config = cli_parse(argc, argv);
    return cli_run_file(&config);
}
