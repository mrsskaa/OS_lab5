#include "console.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_int(char **cursor, int *number){
    char *end;
    errno = 0;
    long value = strtol(*cursor, &end, 10);
    if (end == *cursor || errno == ERANGE || value < INT_MIN || value > INT_MAX ||
        (*end != '\0' && !isspace((unsigned char)*end))) {
        return 0;
    }
    *cursor = end;
    *number = (int)value;
    return 1;
}

int read_command(int *command, int *a, int *b){
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;
    while ((length = getline(&line, &capacity, stdin)) >= 0) {
        char *cursor = line;
        while (isspace((unsigned char)*cursor)) {
            ++cursor;
        }
        if (*cursor == '\0' && cursor == line + length) {
            continue;
        }

        int valid = memchr(line, '\0', (size_t)length) == NULL &&
                    read_int(&cursor, command);
        if (valid && (*command == 1 || *command == 2)) {
            valid = read_int(&cursor, a) && read_int(&cursor, b);
        } else {
            valid = valid && *command == 0;
        }
        while (isspace((unsigned char)*cursor)) {
            ++cursor;
        }
        if (valid && *cursor == '\0') {
            free(line);
            return 1;
        }
        fprintf(stderr, "Invalid command. Use 0, 1 A B or 2 A B.\n");
    }
    free(line);
    if (ferror(stdin) || !feof(stdin)) {
        perror("getline");
        return -1;
    }
    return 0;
}

int print_result(int command, int a, int b, contract_fn prime_count, contract_fn gcf){
    if (a < 1 || b < 1 || (command == 1 && b < a)) {
        fprintf(stderr, "Expected positive arguments and A <= B for PrimeCount.\n");
        return 1;
    }
    int result = command == 1 ? prime_count(a, b) : gcf(a, b);
    if (result < 0) {
        fprintf(stderr, "PrimeCount: could not allocate the sieve.\n");
        return 1;
    }

    const char *name = command == 1 ? "PrimeCount" : "GCF";
    if (printf("%s(%d, %d) = %d\n", name, a, b, result) < 0 || fflush(stdout) == EOF) {
        perror("stdout");
        return 0;
    }
    return 1;
}

int print_implementation(int implementation){
    if (printf("Implementation %d\n", implementation) < 0 || fflush(stdout) == EOF) {
        perror("stdout");
        return 0;
    }
    return 1;
}