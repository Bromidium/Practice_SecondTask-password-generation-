/*
lib_main.c - реализация функций

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
#define MAX_ALPHABET_SIZE 512
#define DEFAULT_PASSWORD_LENGTH 8

void init_options(GenOptions* opts) {
    memset(opts, 0, sizeof(GenOptions)); // все флаги = 0
    opts->count = 1;
}

static int is_numeric(const char* s) {
    if (s == NULL || *s == '\0') return 0;
    while (*s != '\0') {
        if (*s < '0' || *s > '9') return 0;
        s++;
    }
    return 1;
}

static int is_sep(char c, const char* seps, int cnt) {
    for (int i = 0; i < cnt; i++) {
        if (c == seps[i]) return 1;
    }
    return 0;
}

static int match(const char* arg, const char* opt, const char* seps, int cnt, char** val) {
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

    // значение слитно с именем опции
    *val = (char*)&arg[len];
    return 1;
}

int parse_args(int argc, char** argv, GenOptions* opts) {
    char seps[10] = { '=', ':' };
    int sep_cnt = 2;

    for (int i = 1; i < argc; i++) {
        char* val = NULL;

        if (match(argv[i], "-d", seps, sep_cnt, &val)) {
            char c = (val && *val != '\0') ? *val : (i + 1 < argc ? argv[i + 1][0] : 0);
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
            continue;
        }
        else if (match(argv[i], "-D", seps, sep_cnt, &val)) {
            char c = (val && *val != '\0') ? *val : (i + 1 < argc ? argv[i + 1][0] : 0);
            if (c != 0) {
                seps[0] = c;
                sep_cnt = 1;
            }
            continue;
        }
        else if (match(argv[i], "-minl", seps, sep_cnt, &val)) {
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
            opts->min_len = atoi(val);
        }
        // обработка максммальной длины
        else if (match(argv[i], "-maxl", seps, sep_cnt, &val)) {
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
            opts->max_len = atoi(val);
        }
        // обработка точной длины
        else if (match(argv[i], "-n", seps, sep_cnt, &val)) {
            if (opts->has_n) {
                fprintf(stderr, "Error: duplicate option -n\n");
                return -1;
            }
            opts->has_n = 1;

            if (!val && i + 1 < argc) val = argv[++i];
            if (!is_numeric(val)) {
                fprintf(stderr, "Error: -n requires a numeric value\n");
                return -1;
            }
            opts->exact_len = atoi(val);
        }
        // обработка количества паролей
        else if (match(argv[i], "-c", seps, sep_cnt, &val)) {
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
                fprintf(stderr, "Error: -c must be greater than 0\n");
                return -1;
            }
        }
        // обработка пользовательского алфавитв -a
        else if (match(argv[i], "-a", seps, sep_cnt, &val)) {
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
            opts->custom_alpha = val;
        }
        // обработка наборов символов -C
        else if (match(argv[i], "-C", seps, sep_cnt, &val)) {
            if (opts->has_C) {
                fprintf(stderr, "Error: duplicate option -C\n");
                return -1;
            }
            opts->has_C = 1;

            if (!val && i + 1 < argc) val = argv[++i];
            if (!val || *val == '\0') {
                fprintf(stderr, "Error: -C requires a value\n");
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
            // установка флагов использования наборов
            for (int j = 0; val[j] != '\0'; j++) {
                if (val[j] == 'a') opts->use_lower = 1;
                else if (val[j] == 'A') opts->use_upper = 1;
                else if (val[j] == 'D') opts->use_digit = 1;
                else if (val[j] == 'S') opts->use_spec = 1;
            }
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
    // -a несовместим с -C
    if (opts->has_a && opts->has_C) {
        fprintf(stderr, "Error: options -a and -C are incompatible\n");
        return -1;
    }

    // -minl и -maxl задаются только вместе
    if (opts->has_min && !opts->has_max) {
        fprintf(stderr, "Error: if -minl is specified, -maxl must also be specified\n");
        return -1;
    }
    if (opts->has_max && !opts->has_min) {
        fprintf(stderr, "Error: if -maxl is specified, -minl must also be specified\n");
        return -1;
    }

    // -minl не должен быть больше -maxl
    if (opts->has_min && opts->has_max && opts->min_len > opts->max_len) {
        fprintf(stderr, "Error: minimum length (-minl) is greater than maximum length (-maxl)\n");
        return -1;
    }

    // длина пароля должна быть положительной - проверка заранее, чтобы не выводить часть паролей в generate, а потом упасть с ошибкой посреди вывода
    if (opts->has_n && opts->exact_len <= 0) {
        fprintf(stderr, "Error: password length (-n) must be greater than 0\n");
        return -1;
    }
    if (opts->has_min && opts->min_len <= 0) {
        fprintf(stderr, "Error: minimum length (-minl) must be greater than 0\n");
        return -1;
    }

    return 0;
}

int generate_passwords(const GenOptions* opts) {
    char alphabet[MAX_ALPHABET_SIZE] = {0};
    int alpha_len = 0;

    if (opts->has_a) {
        if (strlen(opts->custom_alpha) >= MAX_ALPHABET_SIZE) {
            fprintf(stderr, "Error: custom alphabet is too long (max %d characters)\n", MAX_ALPHABET_SIZE - 1);
            return -1;
        }
        strcpy(alphabet, opts->custom_alpha);
        alpha_len = strlen(alphabet);
    }
    else {
        int use_lower = opts->has_C ? opts->use_lower : 1;
        int use_upper = opts->has_C ? opts->use_upper : 1;
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
        int len;
        if (opts->has_n) {
            len = opts->exact_len;
        }
        else if (opts->has_min && opts->has_max) {
            len = opts->min_len + rand() % (opts->max_len - opts->min_len + 1);
        }
        else {
            len = DEFAULT_PASSWORD_LENGTH; // если нет, то дефолт
        }
        // вывод посимвольно
        for (int j = 0; j < len; j++) {
            putchar(alphabet[rand() % alpha_len]);
        }
        putchar('\n');
    }

    return 0;
}