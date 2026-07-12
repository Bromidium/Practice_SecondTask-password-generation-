/*
main.c - главный модуль программы. 

Теплых Семён Сергеевич
МК-101
*/

#include <stdio.h>
#include "lib_main.h"

int main(int argc, char** argv) {
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
