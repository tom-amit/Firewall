//
// Created by karkt on 3/20/2022.
//

#include "../include/BadParse.h"
/** Constructor (C++ STL string, int, int).
 *  @param msg The error message
 *  @param err_num Error number
 *  @param err_off Error offset
 */
BadParse::BadParse(const std::string& msg, int err_num, int err_off):
        error_number(err_num),
        error_offset(err_off),
        error_message(msg)
{}
BadParse::BadParse(const std::string& failedAction,const std::string& where, int err_num, int err_off):error_number(err_num),
                                                                                                       error_offset(err_off),
                                                                                                       error_message("Failed to parse " + failedAction + ": \"" + where + "\"")
                                                                                                       {
}

/** Destructor.
 *  Virtual to allow for subclassing.
 */
BadParse::~BadParse() throw () {}

/** Returns a pointer to the (constant) error description.
 *  @return A pointer to a const char*. The underlying memory
 *  is in possession of the Except object. Callers must
 *  not attempt to free the memory.
 */
const char* BadParse::what() const throw () {
    return error_message.c_str();
}

/** Returns error number.
 *  @return #error_number
 */
int BadParse::getErrorNumber() const throw() {
    return error_number;
}

/**Returns error offset.
 * @return #error_offset
 */
int BadParse::getErrorOffset() const throw() {
    return error_offset;
}

