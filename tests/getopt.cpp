#include "../getoptpp.hpp"

int main(int argc, char *argv[]) {
  using namespace libjh::utility;
  options opt{
    {'h', "help", "long form help description", flags_e::none},
    {'m', "mandatory_long_option", "mandatory long option description", flags_e::mandatory_arg},
    {'o', "optional_long_option", "optional long option description", flags_e::none}//,
   // {short_opt_t{'a'}, long_opt_t{"aaaaaaaa"}, }
  };

}