#include <stdio.h>

// C function that can be called from FCL
double fcl_add(double a, double b) {
    return a + b;
}

double fcl_multiply(double a, double b) {
    return a * b;
}

const char* fcl_greet(const char* name) {
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "Hello from C, %s!", name);
    return buffer;
}

int fcl_factorial(int n) {
    if (n <= 1) return 1;
    return n * fcl_factorial(n - 1);
}
