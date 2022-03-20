//
// Created by karkt on 3/20/2022.
//
#include <iostream>

#ifndef RULESDAST_BADPARSE_H
#define RULESDAST_BADPARSE_H


class BadParse : public std::exception {
    const char* file;
    int line;
    const char* func;
    const char* info;

public:
    BadParse(const char* msg, const char* file_, int line_, const char* func_, const char* info_ = "");
    const char* get_file();
    int get_line();
    const char* get_func();
    const char* get_info();

};


#endif //RULESDAST_BADPARSE_H
