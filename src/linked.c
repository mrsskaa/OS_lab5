#include "console.h"
#include "contracts.h"

#include <stdio.h>
#include <stdlib.h>

int main(void){
    if (!print_implementation(1)) {
        return EXIT_FAILURE;
    }

    Command command;
    int status;
    while ((status = read_command(&command)) > 0) {
        if (command.type == 0) {
            fprintf(stderr, "Switching libraries is available in dynamic.\n");
            continue;
        }
        if (!print_result(command, PrimeCount, GCF)) {
            return EXIT_FAILURE;
        }
    }
    return status < 0;
}
