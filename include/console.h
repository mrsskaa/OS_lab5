#ifndef CONSOLE_H
#define CONSOLE_H

typedef struct {
    int type;
    int a;
    int b;
} Command;

typedef int (*contract_fn)(int, int);

int read_command(Command *command);
int print_result(Command command, contract_fn prime_count, contract_fn gcf);
int print_implementation(int implementation);

#endif
