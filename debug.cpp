#ifndef libjh_debug_cpp
#define libjh_debug_cpp
#include "debug.hpp"
#include <iomanip>
#include <iostream>
#include <string>
namespace libjh {
namespace logging {

void header(bool newLine, int tabs) {
  if (newLine)
    ::std::cout << "\n";
  while (tabs) {
    ::std::cout << "\t";
    tabs--;
  }
}

void dbg(const bool &newLine, const int &tabs,
         const ::std::string &description) {
  header(newLine, tabs);
  ::std::cout << description;
}
template <printable T>
void dbg(const bool &newLine, const int &tabs, const ::std::string &description,
         T data) {
  header(newLine, tabs);
  ::std::cout << description << ::std::fixed << ::std::setprecision(13) << data;
}
template void dbg(const bool &, const int &, const ::std::string &, const int);

template void dbg<::std::string>(const bool &, const int &,
                                 const ::std::string &, ::std::string);



} // namespace logging
} // namespace libjh

#endif