/*
lib_main.c - главный модуль библиотеки. 

Теплых Семён Сергеевич 
МК-101
*/

#include "lib_main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SET_LOWER "abcdefghijklmnopqrstuvwxyz"
#define SET_UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SET_DIGIT "0123456789"
#define SET_SPEC  "!@#$%^&*()-_=+[]{}|;:,.<>?"

void init_options(GenOptions* opts) {
    memset(opts, 0, sizeof(GenOptions)); // все флаги = 0
    opts->count = 1; 
}

// строка - числа
static int is_numeric(const char* s) {
    if (s == NULL || *s == '\0') {
        return 0; 
    }
    while (*s != '\0') {
        if (*s < '0' || *s > '9') {
            return 0; 
        }
        s++;
    }
    return 1;
}

// смотрим разделитеди
static int is_sep(char c, const char* seps, int cnt) {
    for (int i = 0; i < cnt; i++) {
        if (c == seps[i]) {
            return 1;
        }
    }
    return 0;
}
// заглушка!!! дописать
int parse_args(int argc, char** argv, GenOptions* opts) {
    (void)argc; (void)argv; (void)opts; // параметры пока не надо
    return 0;
}

int validate_options(const GenOptions* opts) {
    (void)opts;
    return 0;
}

int generate_passwords(const GenOptions* opts) {
    (void)opts;
    printf("There is no generation yet\n");
    return 0;
}
