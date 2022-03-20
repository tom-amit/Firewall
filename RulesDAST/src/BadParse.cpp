//
// Created by karkt on 3/20/2022.
//

#include "../include/BadParse.h"

BadParse::BadParse(const char* msg, const char* file_, int line_, const char* func_, const char* info_) : std::exception(msg),
                                                                                                     file (file_),
                                                                                                     line (line_),
                                                                                                     func (func_),
                                                                                                     info (info_)
{}

const char* BadParse::get_file() { return file; }
int BadParse::get_line() { return line; }
const char* BadParse::get_func() { return func; }
const char* BadParse::get_info() { return info; }