#ifndef LIBJH_DEBUG_HPP
#define LIBJH_DEBUG_HPP

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>



namespace libjh {
namespace logging {

class debug_string_long {
    public:
    operator std::string ();
};

class debug_string_short {
    public:
    operator std::string ();
};


template <class T>
concept printable = 
    std::is_integral<T>::value 
    or std::is_floating_point<T>::value 
    or std::is_same<T, std::string>::value
    or std::is_convertible<T, std::string>::value;
void dbg(const bool &newLine, const int &tabs, const std::string &description);

template <printable T>
//template <class T>
void dbg(const bool &newLine, const int &tabs, const std::string description, const T data);

void header(bool, int);

} // namespace logging
} // namespace libjh

#endif
