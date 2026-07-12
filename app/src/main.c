/*
main.c - главный модуль программы. 

Теплых Семён Сергеевич
МК-101
*/

#include <stdio.h>
#include "lib_main.h"

void print_help() {
    printf("-minl N   minimum password length\n");
    printf("-maxl N   maximum password length\n");
    printf("-n N      exact password length (incompatible with -minl/-maxl)\n");
    printf("-c N      number of passwords (default 1)\n");
    printf("-a ALPH   custom alphabet (incompatible with -C)\n");
    printf("-C CHARS  character sets: a, A, D, S\n");
    printf("-d CHAR   add separator\n");
    printf("-D CHAR   replace separators\n");
    printf("-h        show this help\n");
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0) {
            print_help();
            return 0;
        }
    }
    GenOptions opts;
    init_options(&opts);
    int res = parse_args(argc, argv, &opts);
    if (res != 0) {
        return 1;
    }
    if (validate_options(&opts) != 0) {
        return 1;
    }
    if (generate_passwords(&opts) != 0) {
        return 1;
    }

    return 0;
}
