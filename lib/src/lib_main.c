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

static int match(const char* arg, const char* opt, const char* seps, int cnt, char** val, int numeric) {
    int len = (int)strlen(opt);
    if (strncmp(arg, opt, len) != 0) return 0;

    char next = arg[len];
    if (next == '\0') {
        *val = NULL; // ждем значение в след аргументе
        return 1;
    }
    if (is_sep(next, seps, cnt)) {
        *val = (char*)&arg[len + 1]; // значение через разделитель
        return 1;
    }
    // опция числовая, следующий символ не цифра - другая опция
    if (numeric && (next < '0' || next > '9')) {
        return 0;
    }

    *val = (char*)&arg[len]; 
    return 1;
}

int parse_args(int argc, char** argv, GenOptions* opts) {
    char seps[10] = { '=', ':' };
    int sep_cnt = 2;
    // первый проход, сбор разделителей
    for (int i = 1; i < argc; i++) {
        char* val = NULL;
        char c = 0;
        if (match(argv[i], "-d", seps, sep_cnt, &val, 0)) {
            c = (val && *val != '\0') ? *val : (i + 1 < argc ? argv[i + 1][0] : 0);
        }
        else if (match(argv[i], "-D", seps, sep_cnt, &val, 0)) {
            c = (val && *val != '\0') ? *val : (i + 1 < argc ? argv[i + 1][0] : 0);
            if (c != 0) {
                seps[0] = c; // замена первого разделителя
                sep_cnt = 1; // остальные убрать
            }
            continue;
        }
        else {
            continue; // skip, если не связаны с -d -D
        }
        // нет разделителя - добавляем
        if (c != 0 && sep_cnt < 10) {
            int found = 0;
            for (int j = 0; j < sep_cnt; j++) {
                if (seps[j] == c) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                seps[sep_cnt++] = c;
            }
        }
    }
    // второй проход для разбора самих опций
    for (int i = 1; i < argc; i++) {
        char* val = NULL;

        // обработка минимальной длины
        if (match(argv[i], "-minl", seps, sep_cnt, &val, 1)) {
            if (opts->has_min) {
                fprintf(stderr, "Error: duplicate option -minl\n");
                return -1;
            }
            opts->has_min = 1;

            if (!val && i + 1 < argc) val = argv[++i];
            if (!is_numeric(val)) {
                fprintf(stderr, "Error: -minl requires a numeric value\n");
                return -1;
            }
            opts->minl = atoi(val);
        }
        // обработка максммальной длины
        else if (match(argv[i], "-maxl", seps, sep_cnt, &val, 1)) {
            if (opts->has_max) {
                fprintf(stderr, "Error: duplicate option -maxl\n");
                return -1;
            }
            opts->has_max = 1;

            if (!val && i + 1 < argc) val = argv[++i];
            if (!is_numeric(val)) {
                fprintf(stderr, "Error: -maxl requires a numeric value\n");
                return -1;
            }
            opts->maxl = atoi(val);
        }
        // обработка точной длины
        else if (match(argv[i], "-n", seps, sep_cnt, &val, 1)) {
            if (opts->has_n) {
                fprintf(stderr, "Error: duplicate -n option\n");
                return -1;
            }
            opts->has_n = 1;

            if (!val && i + 1 < argc) val = argv[++i];
            if (!is_numeric(val)) {
                fprintf(stderr, "Error: -n requires a numeric value\n");
                return -1;
            }
            opts->exactl = atoi(val);
        }
        // обработка количества паролей
        else if (match(argv[i], "-c", seps, sep_cnt, &val, 1)) {
            if (opts->has_c) {
                fprintf(stderr, "Error: duplicate option -c\n");
                return -1;
            }
            opts->has_c = 1;

            if (!val && i + 1 < argc) val = argv[++i];
            if (!is_numeric(val)) {
                fprintf(stderr, "Error: -c requires a numeric value\n");
                return -1;
            }
            opts->count = atoi(val);
            if (opts->count <= 0) {
                fprintf(stderr, "Error: the number of passwords (-c) must be greater than 0\n");
                return -1;
            }
        }
        // остальное пока игнор
    }

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
