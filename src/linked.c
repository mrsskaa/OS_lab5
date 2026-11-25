#include "console.h"
#include "contracts.h"

#include <stdio.h>
#include <stdlib.h>

int main(void){
    if (!print_implementation(1)) {
        return EXIT_FAILURE;
    }

    int command, a = 0, b = 0;
    int status;
    while ((status = read_command(&command, &a, &b)) > 0) {
        if (command == 0) {
            fprintf(stderr, "Switching libraries is available in dynamic.\n");
        } else if (!print_result(command, a, b, PrimeCount, GCF)) {
            return EXIT_FAILURE;
        }
    }
    return status == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
