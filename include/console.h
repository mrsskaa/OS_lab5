#ifndef CONSOLE_H
#define CONSOLE_H

typedef int (*contract_fn)(int, int);

/* Return 0 at EOF, 1 on a valid command and -1 on a read error. */
int read_command(int *command, int *a, int *b);
int print_result(int command, int a, int b, contract_fn prime_count, contract_fn gcf);
int print_implementation(int implementation);

#endif
