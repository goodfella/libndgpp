cmake_minimum_required(VERSION 3.5.1)
project(libndgpp CXX)

string(LENGTH ${CMAKE_SOURCE_DIR} LIBNDGPP_SOURCE_DIR_LENGTH)

configure_file(include/libndgpp/parent_directory_name_length.hpp.in
  include/libndgpp/parent_directory_name_length.hpp)

set(LIBNDGPP_SOURCE_DIR_LENGTH ${LIBNDGPP_SOURCE_DIR_LENGTH} PARENT_SCOPE)

set(ndgpp_compile_flags -pedantic -Wall -Werror)
add_library(ndgpp SHARED
  src/net/ipv4_address.cpp)
target_compile_options(ndgpp PUBLIC -std=c++14)
target_compile_options(ndgpp PRIVATE ${ndgpp_compile_flags})
target_include_directories(ndgpp PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include)

if (LIBNDGPP_UNIT_TESTS)
  add_subdirectory(test)
endif()
