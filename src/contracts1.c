#include "contracts.h"

int PrimeCount(int A, int B){
    if (A < 1 || B < A) {
        return -1;
    }
    int count = 0;
    if (B < 2) {
        return count;
    }
    for (int number = A < 2 ? 2 : A; ; ++number) {
        int divisor = 2;
        while (divisor < number && number % divisor != 0) {
            ++divisor;
        }
        if (divisor == number) {
            ++count;
        }
        if (number == B) {
            break;
        }
    }
    return count;
}

int GCF(int A, int B){
    if (A < 1 || B < 1) {
        return -1;
    }
    while (B != 0) {
        int remainder = A % B;
        A = B;
        B = remainder;
    }
    return A;
}
