#ifndef libjh_file_operations_hpp
#define libjh_file_operations_hpp
#include <string>
#include <vector>
#include <system_error>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace libjh {
namespace files {
/** \brief Returns contents of a file as line strings */
std::vector<std::string> lines_from_file(const std::filesystem::path pathname,
                                         std::error_code err) {


  if (std::filesystem::exists(pathname, err)) {
    std::cout << "exists";
    if (err) {
      std::cerr << err << std::endl;
    } else {
      std::ifstream infile{pathname};
    }
  }

return std::vector<std::string> {};
                                         };

} // namespace files
} // namespace libjh

#endif