#libjh
cmake_minimum_required(VERSION 3.20)
project(libjh
        VERSION 0.0.1
        DESCRIPTION "Utility library"
        LANGUAGES C CXX
)
option(test "Run tests" ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
add_library( jh
        debug.cpp
        phy.hpp
)


set_target_properties(jh PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        PUBLIC_HEADER "base.hpp;debug.hpp;environment.hpp;configuration.hpp;enum_flag.hpp;file_operations.hpp;filesystem_macro.hpp;getoptpp.hpp;phy.hpp"
)
include (GNUInstallDirs)
include_directories (./)

add_executable ( getopt_cpp_test
        ./tests/getopt.cpp )
add_executable ( getopt_c_test
        ./tests/getopt.c)
add_test (getopt_cpp getopt_test)




install(TARGETS jh
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libjh/)
configure_file(libjh.pc.in libjh.pc @ONLY)
install ( FILES ${CMAKE_BINARY_DIR}/libjh.pc
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig/)
