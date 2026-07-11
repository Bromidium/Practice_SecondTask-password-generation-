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
    printf("first pass\n");
    int res = parse_args(argc, argv, &opts);
    if (res != 0) {
        printf("Error parsing arguments\n");
        return 1;
    }
    printf("Arguments ok\n");

    return 0;
}
