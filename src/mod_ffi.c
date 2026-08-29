#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dlfcn.h>
#include "value.h"
#include "vm.h"
#include "hashmap.h"

#define _POSIX_C_SOURCE 200809L

typedef struct {
    bool had_error;
    bool runtime_error;
    char message[256];
} ErrorReport;

static ErrorReport global_error = {false, false, ""};

static inline void error_runtime_local(ErrorReport* report, const char* format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "Runtime Error: ");
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    report->runtime_error = true;
}

static HashMap* loaded_libraries = NULL;

typedef Value (*NativeFunction)(Value*, int);

typedef struct {
    void* handle;
    char* name;
} FfiLibrary;

void ffi_init() {
    if (!loaded_libraries) {
        loaded_libraries = hashmap_create();
    }
}

Value ffi_load_lib(const char* path) {
    if (!loaded_libraries) ffi_init();

    // Check if already loaded
    Value* cached = hashmap_get(loaded_libraries, path);
    if (cached) {
        return *cached;
    }

    void* handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        error_runtime_local(&global_error, "Failed to load library %s: %s", path, dlerror());
        return NIL_VAL;
    }

    FfiLibrary* lib = (FfiLibrary*)malloc(sizeof(FfiLibrary));
    lib->handle = handle;
    lib->name = strdup(path);

    // Simplified: just return nil for now
    // Real impl would wrap the library handle
    (void)lib; // Suppress unused warning
    return NIL_VAL;
}

Value ffi_get_function(Value lib_val, const char* func_name) {
    // Simplified placeholder
    // Real impl would resolve symbol and return a callable
    (void)lib_val;
    (void)func_name;
    return NIL_VAL;
}

void ffi_cleanup() {
    if (loaded_libraries) {
        // In real impl, iterate and dlclose all handles
        free(loaded_libraries);
        loaded_libraries = NULL;
    }
}
