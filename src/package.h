#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char* name;
    char* version;
    char* edition;
} PackageInfo;

typedef struct {
    char* name;
    char* version;
    char** features;
    size_t feature_count;
} Dependency;

typedef struct {
    PackageInfo package;
    Dependency* dependencies;
    size_t dependency_count;
} PackageConfig;

PackageConfig* package_load(const char* path);
void package_free(PackageConfig* config);
bool package_resolve_module(const char* module_name, char* out_path, size_t out_size);

#endif
