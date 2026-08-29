#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

// Struct example
typedef struct {
    double x;
    double y;
} Point;

// Function: Create a point
Point* create_point(double x, double y) {
    Point* p = (Point*)malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

// Function: Calculate distance from origin
double point_distance(Point* p) {
    if (!p) return 0.0;
    return sqrt(p->x * p->x + p->y * p->y);
}

// Function: Modify point in place
void modify_point(Point* p, double dx, double dy) {
    if (p) {
        p->x += dx;
        p->y += dy;
    }
}

// Function: Free point
void free_point(Point* p) {
    free(p);
}

// Variadic function: Sum of integers
int sum_variadic(int count, ...) {
    va_list args;
    va_start(args, count);
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    va_end(args);
    return total;
}

// Callback type definition
typedef void (*event_callback)(int value);

// Global callback storage
static event_callback global_callback = NULL;

// Function: Register callback
void register_callback(event_callback cb) {
    global_callback = cb;
}

// Function: Trigger event
void trigger_event(int value) {
    if (global_callback) {
        global_callback(value);
    }
}

// Memory buffer example
void* allocate_buffer(size_t size) {
    return malloc(size);
}

void write_to_buffer(void* buf, const char* data, size_t len) {
    if (buf && data) {
        memcpy(buf, data, len);
    }
}

const char* read_from_buffer(void* buf) {
    return (const char*)buf;
}

void free_buffer(void* buf) {
    free(buf);
}
