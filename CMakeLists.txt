#libjh
cmake_minimum_required(VERSION 3.20)
project(libjh
        VERSION 0.0.1
        DESCRIPTION "Utility library"
        LANGUAGES C CXX
)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
add_library( libjh
        debug.cpp
        environment.hpp
        phy.hpp
        file_operations.hpp
              
)
set(HEADERS
        debug.hpp 
        environment.hpp
        configuration.hpp
        enum_flag.hpp
        file_operations.hpp
        filesystem_macro.hpp
        getoptpp.hpp
        phy.hpp)
set_target_properties(libjh
        PROPERTIES CXX_STANDARD 20
        PREFIX ""
        CXX_STANDARD_REQUIRED YES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        PUBLIC_HEADER HEADERS
)
include (GNUInstallDirs)
include_directories (./)

install(TARGETS libjh
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libjh/)
configure_file(libjh.pc.in libjh.pc @ONLY)
install ( FILES ${CMAKE_BINARY_DIR}/libjh.pc
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})
