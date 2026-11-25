#include "console.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_command(Command *command){
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;

    while ((length = getline(&line, &capacity, stdin)) >= 0) {
        char *cursor = line;
        int values[3];
        int count = 0;
        int valid = memchr(line, '\0', (size_t)length) == NULL;

        while (valid) {
            while (isspace((unsigned char)*cursor)) {
                ++cursor;
            }
            if (*cursor == '\0' || count == 3) {
                break;
            }

            char *end;
            errno = 0;
            long value = strtol(cursor, &end, 10);
            if (end == cursor || errno == ERANGE ||
                value < INT_MIN || value > INT_MAX ||
                (*end != '\0' && !isspace((unsigned char)*end))) {
                valid = 0;
                break;
            }
            values[count++] = (int)value;
            cursor = end;
        }

        if (valid && count == 0) {
            continue;
        }
        if (valid && *cursor == '\0' &&
            ((count == 1 && values[0] == 0) ||
             (count == 3 && (values[0] == 1 || values[0] == 2)))) {
            if (count == 3 &&
                (values[1] < 1 || values[2] < 1 ||
                 (values[0] == 1 && values[2] < values[1]))) {
                fprintf(stderr, "Expected positive arguments and A <= B for PrimeCount.\n");
                continue;
            }
            command->type = values[0];
            command->a = count == 3 ? values[1] : 0;
            command->b = count == 3 ? values[2] : 0;
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


int print_result(Command command, contract_fn prime_count, contract_fn gcf){
    int result;
    const char *name;
    if (command.type == 1) {
        result = prime_count(command.a, command.b);
        name = "PrimeCount";
    } else {
        result = gcf(command.a, command.b);
        name = "GCF";
    }

    if (result < 0) {
        fprintf(stderr, "PrimeCount: could not allocate the sieve.\n");
        return 1;
    }
    if (printf("%s(%d, %d) = %d\n", name, command.a, command.b, result) < 0 ||
        fflush(stdout) == EOF) {
        perror("stdout");
        return 0;
    }
    return 1;
}

int print_implementation(int implementation){
    if (printf("Implementation %d\n", implementation) < 0 ||
        fflush(stdout) == EOF) {
        perror("stdout");
        return 0;
    }
    return 1;
}
