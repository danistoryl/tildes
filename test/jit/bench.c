#include <stdio.h>
#include <stdint.h>

int64_t calculate_sum(int64_t a, int64_t b) {
    return a + b;
}

int64_t loop_unroll(int64_t n) {
    int64_t sum = 0;
    for (int64_t i = n; i > 0; i--) {
        sum += i;
    }
    return sum;
}

void run_benchmark() {
    printf("C Benchmark Ready\n");
}
