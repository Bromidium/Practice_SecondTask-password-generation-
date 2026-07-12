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
    // результаты парсинга
    printf("min_len:   %d (given: %s)\n", opts.minl, opts.has_min ? "yes" : "no");
    printf("max_len:   %d (given: %s)\n", opts.maxl, opts.has_max ? "yes" : "no");
    printf("exact_len: %d (given: %s)\n", opts.exactl, opts.has_n ? "yes" : "no");
    printf("count:     %d (given: %s)\n", opts.count, opts.has_c ? "yes" : "no");
    printf("custom_alpha: %s (given: %s)\n", opts.custom_alph ? opts.custom_alph : "NULL", opts.has_a ? "yes" : "no");
    printf("use_lower: %d, use_upper: %d, use_digit: %d, use_spec: %d (given -C: %s)\n", opts.use_low, opts.use_up, opts.use_digit, opts.use_spec, opts.has_C ? "yes" : "no");

    printf("has_m1: %s, has_m2: %s\n", opts.has_m1 ? "yes" : "no", opts.has_m2 ? "yes" : "no");

    return 0;
}
