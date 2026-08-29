#define _GNU_SOURCE
#include "package.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char* trim_whitespace(char* str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

static char* read_line(FILE* file, char* buffer, size_t size) {
    if (!fgets(buffer, size, file)) return NULL;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return trim_whitespace(buffer);
}

PackageConfig* package_load(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return NULL;
    
    PackageConfig* config = calloc(1, sizeof(PackageConfig));
    if (!config) {
        fclose(file);
        return NULL;
    }
    
    char line[1024];
    char current_section[64] = {0};
    
    while (read_line(file, line, sizeof(line))) {
        if (line[0] == '[' && line[strlen(line) - 1] == ']') {
            strncpy(current_section, line + 1, strlen(line) - 2);
            current_section[strlen(line) - 2] = '\0';
            continue;
        }
        
        if (line[0] == '#' || line[0] == '\0') continue;
        
        char* eq = strchr(line, '=');
        if (!eq) continue;
        
        *eq = '\0';
        char* key = trim_whitespace(line);
        char* value = trim_whitespace(eq + 1);
        
        if (strcmp(current_section, "package") == 0) {
            if (strcmp(key, "name") == 0) {
                config->package.name = strdup(value);
            } else if (strcmp(key, "version") == 0) {
                config->package.version = strdup(value);
            } else if (strcmp(key, "edition") == 0) {
                config->package.edition = strdup(value);
            }
        } else if (strcmp(current_section, "dependencies") == 0) {
            size_t idx = config->dependency_count++;
            config->dependencies = realloc(config->dependencies, 
                config->dependency_count * sizeof(Dependency));
            
            Dependency* dep = &config->dependencies[idx];
            memset(dep, 0, sizeof(Dependency));
            dep->name = strdup(key);
            
            char* ver_start = strchr(value, '"');
            if (ver_start) {
                ver_start++;
                char* ver_end = strchr(ver_start, '"');
                if (ver_end) {
                    *ver_end = '\0';
                    dep->version = strdup(ver_start);
                }
            }
        }
    }
    
    fclose(file);
    return config;
}

void package_free(PackageConfig* config) {
    if (!config) return;
    
    free(config->package.name);
    free(config->package.version);
    free(config->package.edition);
    
    for (size_t i = 0; i < config->dependency_count; i++) {
        free(config->dependencies[i].name);
        free(config->dependencies[i].version);
        for (size_t j = 0; j < config->dependencies[i].feature_count; j++) {
            free(config->dependencies[i].features[j]);
        }
        free(config->dependencies[i].features);
    }
    free(config->dependencies);
    free(config);
}

bool package_resolve_module(const char* module_name, char* out_path, size_t out_size) {
    if (!module_name || !out_path) return false;
    
    if (strcmp(module_name, "System") == 0) {
        snprintf(out_path, out_size, "lib/System/");
        return true;
    }
    
    char* dot = strchr(module_name, '.');
    if (dot) {
        char package_name[256];
        size_t pkg_len = dot - module_name;
        strncpy(package_name, module_name, pkg_len);
        package_name[pkg_len] = '\0';
        
        snprintf(out_path, out_size, "lib/%s/", package_name);
        return true;
    }
    
    snprintf(out_path, out_size, "lib/%s/", module_name);
    return true;
}
