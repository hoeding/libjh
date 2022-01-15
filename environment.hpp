#ifndef libjh_environment_hpp
#define libjh_environment_hpp
#include <vector>
#include <tuple>
#include <cassert>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <memory>
#include <sstream>
#include <cstring>
#include <algorithm>
#include "debug.hpp"

#include <filesystem_macro.hpp>
//using env_t = std::vector<std::tuple<std::string, std::vector<std::string>>>;

namespace libjh {
namespace environment {

//enum class 




std::vector<libjh::fs::path> parse_path(std::string path_temp){
  return std::vector<libjh::fs::path> {};
}
void parse_environment(char ** env){
  
};
class state {

public:
  // shared_ptr



  std::string get_value(std::string key) { return std::string {}; }

private:
  struct env_t{
    std::string key{};
    std::vector<std::string> values{};
  };
  std::vector<env_t> environment {};
  char **c_style = nullptr;
};

//
//
//

std::vector<std::filesystem::path> env_path() {
  struct token_t {
    std::string::size_type start;
    std::string::size_type end;
  };
  std::vector<token_t> tokens;
  std::vector<std::filesystem::path> paths{};
  std::string raw_path{getenv("PATH")};

  std::string::size_type next = 0, prev = 0;

  for (std::string::size_type position = 0; position < raw_path.length();
       position++) {
    std::vector<std::string::size_type> colon_positions{};
    if (raw_path.at(position) == ':') {
    }
  }

  return paths;
};
} // namespace environment
//
//
//


namespace arguments {


std::error_code parse_arguments(int _argc, char * argv[], char * envp []){

return std::error_code {};
}



















// TODO: add tcs
template <class dest_type>
dest_type &string_to(std::string const &s, dest_type &result) {
  std::stringstream stream(s);
  stream >> result;
  if ((!stream) || (stream.fail())) {
    throw std::invalid_argument("error converting string '" + std::string(s) +
                                "'");
  }
  return result;
}

template <class dest_type> dest_type string_to(std::string const &s) {
  dest_type result;
  return string_to(s, result);
}

template <typename> struct is_bool {
  static bool value() { return false; }
};
template <> struct is_bool<bool> {
  static bool value() { return true; }
};

class type_base {
  type_base &operator=(const type_base &);

public:
  const std::string name;
  const std::string description;

  type_base(std::string a_name, std::string a_description)
      : name(a_name), description(a_description) {}
  virtual void parse_and_store(const std::string &s) = 0;
  virtual std::string value() const = 0;
  virtual std::unique_ptr<type_base> clone() const = 0;
  virtual ~type_base() {}
};
template <typename type> class type_impl : public type_base {
private:
  type_impl(const type_impl &src)
      : type_base(src.name, src.description), target(src.target),
        validating_function(src.validating_function) {}
  type_impl &operator=(const type_impl &);
  typedef bool (*validating_function_type)(const type &);
  type &target;
  validating_function_type validating_function;

public:
  type_impl(std::string a_name, std::string a_description, type &a_target,
            validating_function_type a_validating_function = nullptr)
      : type_base(a_name, a_description), target(a_target),
        validating_function(a_validating_function){};
  void parse_and_store(const std::string &s) /*override*/ {
    try {
      const bool is_bool = arguments::is_bool<type>::value();
      if (is_bool && s.empty()) {
        // to avoid directly assigning true
        //(as it will impose additional layer of indirection)
        // so, simply pass it as string
        arguments::string_to("1", target);
      } else {
        arguments::string_to(s, target);
      }
    } catch (std::invalid_argument &e) {
      std::stringstream str;
      str << "'" << s << "' is incorrect input for argument '" << name << "'"
          << " (" << e.what() << ")";
      throw std::invalid_argument(str.str());
    }
    if (validating_function) {
      if (!((validating_function)(target))) {
        std::stringstream str;
        str << "'" << target << "' is invalid value for argument '" << name
            << "'";
        throw std::invalid_argument(str.str());
      }
    }
  }
  template <typename t> static bool is_null_c_str(t &) { return false; }
  static bool is_null_c_str(char *s) { return s == nullptr; }
  std::string value() const /*override*/ {
    std::stringstream str;
    if (!is_null_c_str(target))
      str << target;
    return str.str();
  }
  std::unique_ptr<type_base> clone() const /*override*/ {
    return std::unique_ptr<type_base>(new type_impl(*this));
  }
};

class argument {
private:
  std::unique_ptr<type_base> p_type;
  bool matched_;

public:
  argument(argument const &other)
      : p_type(other.p_type.get() ? (other.p_type->clone()).release()
                                  : nullptr),
        matched_(other.matched_) {}
  argument &operator=(argument a) {
    this->swap(a);
    return *this;
  }
  void swap(argument &other) {
    std::swap(p_type, other.p_type);
    std::swap(matched_, other.matched_);
  }
  template <class type>
  argument(std::string a_name, std::string a_description, type &dest,
           bool (*a_validating_function)(const type &) = nullptr)
      : p_type(new type_impl<type>(a_name, a_description, dest,
                                   a_validating_function)),
        matched_(false) {}
  std::string value() const { return p_type->value(); }
  std::string name() const { return p_type->name; }
  std::string description() const { return p_type->description; }
  void parse_and_store(const std::string &s) {
    p_type->parse_and_store(s);
    matched_ = true;
  }
  bool is_matched() const { return matched_; }
};


class cli_argument_pack {
  typedef std::map<std::string, arguments::argument> args_map_type;
  typedef std::vector<std::string> args_display_order_type;
  typedef std::vector<std::string> positional_arg_names_type;

private:
  args_map_type args_map;
  args_display_order_type args_display_order;
  positional_arg_names_type positional_arg_names;
  std::set<std::string> bool_args_names;

private:
  void add_arg(arguments::argument const &a) {
    std::pair<args_map_type::iterator, bool> result =
        args_map.insert(std::make_pair(a.name(), a));
    if (!result.second) {
      throw std::invalid_argument("argument with name: '" + a.name() +
                                  "' already registered");
    }
    args_display_order.push_back(a.name());
  }

public:
  template <typename type>
  cli_argument_pack &arg(type &dest, std::string const &name,
                         std::string const &description,
                         bool (*validate)(const type &) = nullptr) {
    arguments::argument a(name, description, dest, validate);
    add_arg(a);
    if (arguments::is_bool<type>::value()) {
      bool_args_names.insert(name);
    }
    return *this;
  }

  // Positional means that argument name can be omitted in actual CL
  // only key to match values for parameters with
  template <typename type>
  cli_argument_pack &positional_arg(type &dest, std::string const &name,
                                    std::string const &description,
                                    bool (*validate)(const type &) = nullptr) {
    arguments::argument a(name, description, dest, validate);
    add_arg(a);
    if (arguments::is_bool<type>::value()) {
      bool_args_names.insert(name);
    }
    positional_arg_names.push_back(name);
    return *this;
  }

  void parse(std::size_t argc, char const *argv[]) {
    {
      std::size_t current_positional_index = 0;
      for (std::size_t j = 1; j < argc; j++) {
        arguments::argument *pa = nullptr;
        std::string argument_value;

        const char *const begin = argv[j];
        const char *const end = begin + std::strlen(argv[j]);

        const char *const assign_sign = std::find(begin, end, '=');

        struct throw_unknown_parameter {
          static void _(std::string const &location) {
            throw std::invalid_argument(
                std::string("unknown parameter starting at:'") + location +
                "'");
          }
        };
        // first try to interpret it like parameter=value string
        if (assign_sign != end) {
          std::string name_found = std::string(begin, assign_sign);
          args_map_type::iterator it = args_map.find(name_found);

          if (it != args_map.end()) {
            pa = &((*it).second);
            argument_value = std::string(assign_sign + 1, end);
          } else {
            throw_unknown_parameter::_(argv[j]);
          }
        }
        // then see is it a named flag
        else {
          args_map_type::iterator it = args_map.find(argv[j]);
          if (it != args_map.end()) {
            pa = &((*it).second);
            argument_value = "";
          }
          // then try it as positional argument without name specified
          else if (current_positional_index < positional_arg_names.size()) {
            std::stringstream str(argv[j]);
            args_map_type::iterator found_positional_arg = args_map.find(
                positional_arg_names.at(current_positional_index));
            // TODO: probably use of smarter assert would help here
            assert(
                            found_positional_arg !=
                            args_map
                                .end() /*&&"positional_arg_names and args_map are out of sync"*/);
            if (found_positional_arg == args_map.end()) {
              throw std::logic_error(
                  "positional_arg_names and args_map are out of sync");
            }
            pa = &((*found_positional_arg).second);
            argument_value = argv[j];

            current_positional_index++;
          } else {
            // TODO: add tc to check
            throw_unknown_parameter::_(argv[j]);
          }
        }
        assert(pa);
        if (pa->is_matched()) {
          throw std::invalid_argument(
              std::string("several values specified for: '") + pa->name() +
              "' argument");
        }
        pa->parse_and_store(argument_value);
      }
    }
  }
  std::string usage_string(const std::string &binary_name) const {
    std::string command_line_params;
    std::string summary_description;

    for (args_display_order_type::const_iterator it =
             args_display_order.begin();
         it != args_display_order.end(); ++it) {
      const bool is_bool = (0 != bool_args_names.count((*it)));
      args_map_type::const_iterator argument_it = args_map.find(*it);
      // TODO: probably use of smarter assert would help here
      assert(
          argument_it !=
          args_map
              .end() /*&&"args_display_order and args_map are out of sync"*/);
      if (argument_it == args_map.end()) {
        throw std::logic_error(
            "args_display_order and args_map are out of sync");
      }
      const arguments::argument &a = (*argument_it).second;
      command_line_params += " [" + a.name() + (is_bool ? "" : "=value") + "]";
      summary_description += " " + a.name() + " - " + a.description() + " (" +
                             a.value() + ")" + "\n";
    }

    std::string positional_arg_cl;
    for (positional_arg_names_type::const_iterator it =
             positional_arg_names.begin();
         it != positional_arg_names.end(); ++it) {
      positional_arg_cl += " [" + (*it);
    }
    for (std::size_t i = 0; i < positional_arg_names.size(); ++i) {
      positional_arg_cl += "]";
    }
    command_line_params += positional_arg_cl;
    std::stringstream str;
    str << " Program usage is:"
        << "\n"
        << " " << binary_name << command_line_params << "\n"
        << "\n"
        << " where:"
        << "\n"
        << summary_description;
    return str.str();
  }
}; // class cli_argument_pack

inline void parse_cli_arguments(int argc, const char *argv[],
                                cli_argument_pack cli_pack) {
  bool show_help = false;
  cli_pack.arg(show_help, "-h", "show this message");

  bool invalid_input = false;
  try {
    cli_pack.parse(argc, argv);
  } catch (std::exception &e) {
    std::cerr << "error occurred while parsing command line."
              << "\n"
              << "error text: " << e.what() << "\n"
              << std::flush;
    invalid_input = true;
  }
  if (show_help || invalid_input) {
    std::cout << cli_pack.usage_string(argv[0]) << std::flush;
    std::exit(0);
  }
}
inline void parse_cli_arguments(int argc, char *argv[],
                                cli_argument_pack cli_pack) {
  parse_cli_arguments(argc, const_cast<const char **>(argv), cli_pack);
}

} // namespace arguments

} // namespace libjh

#endif