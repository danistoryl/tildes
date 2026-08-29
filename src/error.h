#ifndef INTERPRETER_ERROR_H
#define INTERPRETER_ERROR_H

#include "types.h"
#include <stdio.h>
#include <stdarg.h>

// --- Error Reporting ---
typedef struct {
    bool had_error;
    bool runtime_error;
    char message[256];
} ErrorReport;

static inline void error_init(ErrorReport* report) {
    report->had_error = false;
    report->runtime_error = false;
    report->message[0] = '\0';
}

static inline void error_report(ErrorReport* report, int line, const char* msg) {
    fprintf(stderr, "[Line %d] Error: %s\n", line, msg);
    report->had_error = true;
}

static inline void error_runtime(ErrorReport* report, const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "Runtime Error: ");
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    report->runtime_error = true;
}

static inline void error_type(ErrorReport* report, int line, const char* expected, const char* got) {
    fprintf(stderr, "[Line %d] Type Error: Expected %s but got %s\n", line, expected, got);
    report->had_error = true;
}

#endif
