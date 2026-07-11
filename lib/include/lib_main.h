/*
lib_main.h - заголовочный файл модуля lib_main.c. 

Теплых Семён Сергеевич
МК-101
*/

#ifndef LIB_MAIN_H
#define LIB_MAIN_H

typedef struct {
    int minl;       
    int maxl;       
    int exactl;     
    int count;         
    char* custom_alph; 
    // флаги использования наборов символов
    int use_low;      
    int use_up;      
    int use_digit;      
    int use_spec;       
    // флаги наличия 
    int has_min;
    int has_max;
    int has_n;
    int has_c;
    int has_C;
    int has_a;
    // флаги для проверки, что -m1 -m2 вместе
    int has_m1;
    int has_m2;
} GenOptions;

void init_options(GenOptions* opts);
int parse_args(int argc, char** argv, GenOptions* opts);
int validate_options(const GenOptions* opts);
int generate_passwords(const GenOptions* opts);
#endif

