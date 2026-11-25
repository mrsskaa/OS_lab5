#include "console.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void *handle;
    contract_fn prime_count;
    contract_fn gcf;
} library;

static int close_library(library loaded)
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

static int load_library(const char *path, library *loaded)
{
    void *handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        fprintf(stderr, "dlopen %s: %s\n", path, dlerror());
        return 0;
    }

    library candidate = {handle, find_function(handle, "PrimeCount"),
                                 find_function(handle, "GCF")};
    if (candidate.prime_count == NULL || candidate.gcf == NULL) {
        return close_library(candidate) ? 0 : -1;
    }
    *loaded = candidate;
    return 1;
}

int main(void)
{
    const char *paths[] = {LIBRARY_1, LIBRARY_2};
    int active = 0;
    library current;
    if (load_library(paths[active], &current) != 1) {
        return EXIT_FAILURE;
    }

    int result = EXIT_SUCCESS;
    int command, a = 0, b = 0;
    int status = 0;
    if (!print_implementation(active + 1)) {
        result = EXIT_FAILURE;
    }

    while (result == EXIT_SUCCESS && (status = read_command(&command, &a, &b)) > 0) {
        if (command == 0) {
            int next = 1 - active;
            library candidate;
            int loaded = load_library(paths[next], &candidate);
            if (loaded == 0) {
                continue;
            }
            if (loaded < 0) {
                result = EXIT_FAILURE;
                break;
            }
            int closed = close_library(current);
            current = candidate;
            active = next;
            if (!closed || !print_implementation(active + 1)) {
                result = EXIT_FAILURE;
            }
        } else if (!print_result(command, a, b, current.prime_count, current.gcf)) {
            result = EXIT_FAILURE;
        }
    }
    if (!close_library(current) || status < 0) {
        result = EXIT_FAILURE;
    }
    return result;
}
