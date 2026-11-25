#include "contracts.h"
#include <stdlib.h>

int PrimeCount(int A, int B){
    if (A < 1 || B < A) {
        return -1;
    }
    if (B < 2) {
        return 0;
    }

    int64_t limit = B;
    unsigned char *composite = calloc((size_t)limit + 1, sizeof(*composite));
    if (composite == NULL) {
        return -1;
    }

    for (int64_t prime = 2; prime * prime <= limit; ++prime) {
        if (composite[prime] == 0) {
            for (int64_t multiple = prime * prime;
                 multiple <= limit; multiple += prime) {
                composite[multiple] = 1;
            }
        }
    }

    int count = 0;
    for (int64_t number = A < 2 ? 2 : A; number <= limit; ++number) {
        if (composite[number] == 0) {
            ++count;
        }
    }
    free(composite);
    return count;
}

int GCF(int A, int B){
    if (A < 1 || B < 1) {
        return -1;
    }
    int divisor = A < B ? A : B;
    while (A % divisor != 0 || B % divisor != 0) {
        --divisor;
    }
    return divisor;
}
