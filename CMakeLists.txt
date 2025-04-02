cmake_minimum_required(VERSION 3.28)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan_provider.cmake")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/refs/heads/develop2/conan_provider.cmake" "${CMAKE_BINARY_DIR}/conan_provider.cmake")
endif()

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${CMAKE_BINARY_DIR}/conan_provider.cmake")

project(metrics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Weverything -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded)
endif()


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

enable_testing()
file(GLOB_RECURSE TESTS test/*.cpp)
find_package(GTest REQUIRED)
foreach(test_file ${TESTS}) 
  get_filename_component(test_name ${test_file} NAME_WE)
  add_executable(${test_name} ${test_file})
  target_link_libraries(${test_name} PRIVATE ${PROJECT_NAME} gtest::gtest)
  target_compile_options(${test_name} PRIVATE -Wno-global-constructors)
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()