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
        return 1;
    }
    // результаты парсинга
    printf("min_len:   %d (given: %s)\n", opts.minl, opts.has_min ? "yes" : "no");
    printf("max_len:   %d (given: %s)\n", opts.maxl, opts.has_max ? "yes" : "no");
    printf("exact_len: %d (given: %s)\n", opts.exactl, opts.has_n ? "yes" : "no");
    printf("count:     %d (given: %s)\n", opts.count, opts.has_c ? "yes" : "no");

    return 0;
}
