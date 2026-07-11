/*
main.c - главный модуль программы. 

Теплых Семён Сергеевич
МК-101
*/

#include <stdio.h>
#include "lib_main.h"

int main(int argc, char** argv) {
    GenOptions opts;
    // пока для линковки
    init_options(&opts);
    printf("The structure is initialized\n");

    return 0;
}
