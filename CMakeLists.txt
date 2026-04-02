cmake_minimum_required(VERSION 3.16)
project(GameServerPractices LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

find_package(GTest REQUIRED)

add_executable(cpp_behavior_tests
  tests/cpp_behavior_test.cpp
  tests/enum_behavior_test.cpp
  tests/type_traits_test.cpp
)

target_link_libraries(cpp_behavior_tests
  PRIVATE GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cpp_behavior_tests)
