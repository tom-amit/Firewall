//
// Created by karkt on 3/20/2022.
//

#ifndef RULESDAST_BADPARSE_H
#define RULESDAST_BADPARSE_H

#include <iostream>


class BadParse: virtual public std::exception {

protected:

    int error_number;               ///< Error number
    int error_offset;               ///< Error offset
    std::string error_message;      ///< Error message

public:

    /** Constructor (C++ STL string, int, int).
     *  @param msg The error message
     *  @param err_num Error number
     *  @param err_off Error offset
     */
    explicit
    BadParse(const std::string& msg, int err_num, int err_off);

    explicit
    BadParse(const std::string& failedAction,const std::string& where, int err_num = -1, int err_off = 0);
    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~BadParse() noexcept;
    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *  is in possession of the Except object. Callers must
     *  not attempt to free the memory.
     */
    virtual const char* what() const noexcept;

    /** Returns error number.
     *  @return #error_number
     */
    virtual int getErrorNumber() const noexcept;

    /**Returns error offset.
     * @return #error_offset
     */
    virtual int getErrorOffset() const noexcept;

};



#endif //RULESDAST_BADPARSE_H
