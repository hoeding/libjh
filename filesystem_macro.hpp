#ifndef libjh_filesystem_hpp
#define libjh_filesystem_hpp
/** @brief Macro to include the host c++ filesystem implementation */
#if __has_include(<filesystem>)
#include <filesystem>
namespace libjh{
  namespace fs = std::filesystem;}
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace libjh{
  namespace fs = std::experimental::filesystem;}
#else
error "Missing the <filesystem> header."
#endif
#endif