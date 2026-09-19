#include <stdio.h>
#include "log.h"

void warning(const char* message) {
    fprintf(stderr, "\033[33mwarning: %s\033[0m\n", message);
}

void error(const char* message) {
    fprintf(stderr, "\033[31merror: %s\033[0m\n", message);
}