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
    // валидация опций
    res = validate_options(&opts);
    if (res != 0) {
        return 1;
    }
    // результаты
    printf("all options are correct and compatible.\n");
    printf("min_len: %d, max_len: %d, exact_len: %d, count: %d\n", opts.minl, opts.maxl, opts.exactl, opts.count);
    printf("custom_alpha: %s\n", opts.custom_alph ? opts.custom_alph : "NULL");
    printf("use_lower: %d, use_upper: %d, use_digit: %d, use_spec: %d\n", opts.use_low, opts.use_up, opts.use_digit, opts.use_spec);
    printf("has_m1: %d, has_m2: %d\n", opts.has_m1, opts.has_m2);

    return 0;
}
