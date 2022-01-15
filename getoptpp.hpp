#ifndef LIBJH_GETOPTPP_HPP
#define LIBJH_GETOPTPP_HPP
#include <errno.h>
#include <vector>
#include <tuple>

namespace libjh {
    namespace utility {
        using short_opt_t = std::string;
        using long_opt_t = std::string;
        using help_text_t = std::string;
        using value_t = std::any;
        using success = bool;
        enum class is_required_e {
            no_arg,
            optional_arg,
            mandatory_arg
        }
        using options = std::vector<std::tuple <short_opt_t, long_opt_t, help_text_t, value_t, is_required_e>>;
        class program_options_t {
            public:
            success parse_argc_argv (int argc, char ** argv){
                ;
            };
            constexpr parse_program_opts ()
            private:
        
        }
        
    }
}
#endif