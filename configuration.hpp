#ifndef libjh_configuration_hpp
#define libjh_configuration_hpp

#include <mutex>
#include <string>
#include <vector>
#include <sstream>
#include "debug.hpp"
namespace libjh {
namespace configuration {
using size_type = std::string::size_type;
const size_type npos = std::string::npos;
const size_type OOPSIE = 50;
using exclusion_list = std::vector<std::string>;
using namespace libjh::logging;
class state {
  public:
  state(){state{0, nullptr, nullptr};};
  state(int argc, char *argv[], char *envp[]) {

    for (size_type i{0}, eq_pos{0}, oops{0};
         (envp[i] != nullptr) and (oops <= OOPSIE); i++, oops++) {
      std::string this_line{envp[i]};
      eq_pos = this_line.find_first_of("=");
      dbg(false, 0, this_line, eq_pos);

      std::vector<size_type> colon_positions{eq_pos};
      for (size_type cursor_pos{eq_pos}, ooops{0};
           (cursor_pos != npos) and (ooops <= OOPSIE); ooops++) {
        dbg(false, 0, "oops=", oops);
        dbg(false, 0, "ooops=", ooops);
        dbg(false, 0, "cursor_pos=", cursor_pos);

        cursor_pos = this_line.find_first_of(":", cursor_pos);
        if (cursor_pos == std::string::npos) //
        {
          dbg(false, 0, "cursor_pos=npos");
          colon_positions.emplace_back(this_line.size());
        } else {
          colon_positions.emplace_back(cursor_pos);
        }
      };
    }
  };
  template <class T>
  void add_cli_option (T * data, char short_name, std::string long_name, std::string help_description, bool is_mandatory){
    return ;
  };
  template <class T>
  void add_cli_option(T *data, std::string long_name,
                      std::string help_description, bool is_mandatory,
                      exclusion_list exclude) {
    return;
  }

  template <class T>
  void add_cli_option (T * data, std::string long_name, std::string help_description, bool is_mandatory){
    add_cli_option(data, long_name, help_description, is_mandatory, exclusion_list {});
    return ;
  }
  void add_cli_option (std::stringstream data, std::string long_name, std::string help_description, bool is_mandatory){
    return ;
  }
 
  std::string get_env(std::string key);
  void printenv() {
    for (auto iterator : environment_vars) {
      //  std::cout << std::get<0> (iterator) << std::endl;
    }
  }
  void parse_args(int argc, char * argv[]){
    ;
  }
private:
  std::vector<std::string> environment_vars{};
};

} // namespace configuration
} // namespace libjh

#endif