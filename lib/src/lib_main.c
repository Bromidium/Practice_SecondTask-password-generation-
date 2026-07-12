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
        // обработка пользовательского алфавитв -a
        else if (match(argv[i], "-a", seps, sep_cnt, &val, 0)) {
            if (opts->has_a) {
                fprintf(stderr, "Error: duplicate option -a\n");
                return -1;
            }
            opts->has_a = 1;

            if (!val && i + 1 < argc) val = argv[++i];
            if (!val || *val == '\0') {
                fprintf(stderr, "Error: -a requires a value\n");
                return -1;
            }
            opts->custom_alph = val;
        }
        // обработка наборов символов -C
        else if (match(argv[i], "-C", seps, sep_cnt, &val, 0)) {
            if (opts->has_C) {
                fprintf(stderr, "Error: duplicate option -C\n");
                return -1;
            }
            opts->has_C = 1;
            if (!val && i + 1 < argc) val = argv[++i];
            if (!val || *val == '\0') {
                fprintf(stderr, "Error: -С requires a value\n");
                return -1;
            }
            // проверка на валидность и дубликаты 
            int seen[4] = { 0 };
            for (int j = 0; val[j] != '\0'; j++) {
                int idx = -1;
                if (val[j] == 'a') idx = 0;
                else if (val[j] == 'A') idx = 1;
                else if (val[j] == 'D') idx = 2;
                else if (val[j] == 'S') idx = 3;

                if (idx == -1) {
                    fprintf(stderr, "Error: invalid character '%c' in -C\n", val[j]);
                    return -1;
                }
                if (seen[idx]) {
                    fprintf(stderr, "Error: duplicate character '%c' in -C\n", val[j]);
                    return -1;
                }
                seen[idx] = 1;
            }

            // ставим флаги использования наборов
            for (int j = 0; val[j] != '\0'; j++) {
                if (val[j] == 'a') opts->use_low = 1;
                else if (val[j] == 'A') opts->use_up = 1;
                else if (val[j] == 'D') opts->use_digit = 1;
                else if (val[j] == 'S') opts->use_spec = 1;
            }
        }
        // обработка -m1 и -m2
        else if (match(argv[i], "-m1", seps, sep_cnt, &val, 0)) {
            if (opts->has_m1) {
                fprintf(stderr, "Error: duplicate option -m1\n");
                return -1;
            }
            opts->has_m1 = 1;
        }
        else if (match(argv[i], "-m2", seps, sep_cnt, &val, 0)) {
            if (opts->has_m2) {
                fprintf(stderr, "Error: duplicate option -m2\n");
                return -1;
            }
            opts->has_m2 = 1;
        }
    }

    return 0;
}

int validate_options(const GenOptions* opts) {
    // -n несовместима с -minl/-maxl
    if (opts->has_n && (opts->has_min || opts->has_max)) {
        fprintf(stderr, "Error: option -n is incompatible with -minl/-maxl\n");
        return -1;
    }
    // -a несовместима с -C
    if (opts->has_a && opts->has_C) {
        fprintf(stderr, "Error: -a and -C options are incompatible\n");
        return -1;
    }
    // -minl не должна быть больше -maxl
    if (opts->has_min && opts->has_max && opts->minl > opts->maxl) {
        fprintf(stderr, "Error: minl is greater than -maxl\n");
        return -1;
    }
    // есть -m1, должна быть -m2 (обратное вроде не надо)
    if (opts->has_m1 && !opts->has_m2) {
        fprintf(stderr, "Error: if -m1, -m2 must be\n");
        return -1;
    }
    // -m1 -m2 нельзя вместе с -n
    if ((opts->has_m1 || opts->has_m2) && opts->has_n) {
        fprintf(stderr, "Error: -m1/-m2 should not be with -n\n");
        return -1;
    }

    return 0;
}

int generate_passwords(const GenOptions* opts) {
    char alphabet[512] = {0};
    int alpha_len = 0;
    // итоговый алфавит
    if (opts->has_a) {
        // если есть пользовательсикй
        strcpy(alphabet, opts->custom_alph);
        alpha_len = strlen(alphabet);
    }
    else {
        // заздали наборы символов, иначе дефолтные
        int use_lower = opts->has_C ? opts->use_low : 1;
        int use_upper = opts->has_C ? opts->use_up : 1;
        int use_digit = opts->has_C ? opts->use_digit : 1;
        int use_spec = opts->has_C ? opts->use_spec : 1;

        if (use_lower) { strcat(alphabet, SET_LOWER); alpha_len += strlen(SET_LOWER); }
        if (use_upper) { strcat(alphabet, SET_UPPER); alpha_len += strlen(SET_UPPER); }
        if (use_digit) { strcat(alphabet, SET_DIGIT); alpha_len += strlen(SET_DIGIT); }
        if (use_spec) { strcat(alphabet, SET_SPEC);  alpha_len += strlen(SET_SPEC); }
    }
    if (alpha_len == 0) {
        fprintf(stderr, "Error: resulting alphabet is empty\n");
        return -1;
    }
    srand((unsigned int)time(NULL));

    // генерация паролей
    for (int i = 0; i < opts->count; i++) {
        int len = 0;
        // определение длины
        if (opts->has_n) {
            len = opts->exactl;
        }
        else {
            if (opts->has_min && opts->has_max) {
                len = opts->minl + rand() % (opts->maxl - opts->minl + 1);
            }
            else if (opts->has_min) {
                len = opts->minl;
            }
            else if (opts->has_max) {
                len = opts->maxl;
            }
            else {
                len = 8; // если не задано, дефолт длина
            }
        }
        if (len <= 0) {
            fprintf(stderr, "Error: password length must be greater than 0\n");
            return -1;
        }
        // вывод посимвольно
        for (int j = 0; j < len; j++) {
            putchar(alphabet[rand() % alpha_len]);
        }
        putchar('\n');
    }

    return 0;
}
