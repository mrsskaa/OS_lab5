#include "console.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

struct library {
    void *handle;
    contract_fn prime_count;
    contract_fn gcf;
};

static int close_library(struct library loaded)
{
    if (dlclose(loaded.handle) != 0) {
        fprintf(stderr, "Cannot close library: %s\n", dlerror());
        return 0;
    }
    return 1;
}

static contract_fn find_function(void *handle, const char *name)
{
    dlerror();
    contract_fn function = (contract_fn)dlsym(handle, name);
    const char *error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "dlsym %s: %s\n", name, error);
        return NULL;
    }
    if (function == NULL) {
        fprintf(stderr, "dlsym %s: null function address\n", name);
    }
    return function;
}

static int load_library(const char *path, struct library *library)
{
    void *handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        fprintf(stderr, "dlopen %s: %s\n", path, dlerror());
        return 0;
    }

    struct library candidate = {handle, find_function(handle, "PrimeCount"),
                                 find_function(handle, "GCF")};
    if (candidate.prime_count == NULL || candidate.gcf == NULL) {
        return close_library(candidate) ? 0 : -1;
    }
    *library = candidate;
    return 1;
}


int main(void){
    const char *paths[] = {LIBRARY_1, LIBRARY_2};
    int active = 0;
    struct library current;
    if (load_library(paths[active], &current) != 1) {
        return EXIT_FAILURE;
    }

    int failed = !print_implementation(1);
    Command command;
    int status = 0;
    while (!failed && (status = read_command(&command)) > 0) {
        if (command.type != 0) {
            failed = !print_result(command, current.prime_count, current.gcf);
            continue;
        }

        int next = 1 - active;
        struct library candidate;
        int loaded = load_library(paths[next], &candidate);
        if (loaded == 0) {
            continue;
        }
        if (loaded < 0) {
            failed = 1;
            break;
        }

        int closed = close_library(current);
        current = candidate;
        active = next;
        if (!closed || !print_implementation(active + 1)) {
            failed = 1;
        }
    }

    if (!close_library(current) || status < 0) {
        failed = 1;
    }
    return failed;
}
