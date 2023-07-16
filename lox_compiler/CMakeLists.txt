# project(lox)

# set(CMAKE_CXX_STANDARD 17)

# add_library(
#   module
#   SHARED
#   token_type.h
#   token.h
#   error.h
#   scanner.h
# )

# add_executable(lox lox.cc)

# target_link_libraries(lox module)

cmake_minimum_required(VERSION 3.10)
project(jlox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

add_executable(jlox lox.cc)

# Add a custom target for the test
add_custom_target(test-lexing
                  COMMAND echo "testing jlox with test-lexing.lox ..."
                  COMMAND ${CMAKE_BINARY_DIR}/jlox ${CMAKE_SOURCE_DIR}/tests/test-lexing.lox | diff -u --color ${CMAKE_SOURCE_DIR}/tests/test-lexing.lox.expected -
                  DEPENDS jlox
)

add_custom_target(test-lexing2
                  COMMAND echo "testing jlox with test-lexing2.lox ..."
                  COMMAND ${CMAKE_BINARY_DIR}/jlox ${CMAKE_SOURCE_DIR}/tests/test-lexing2.lox | diff -u --color ${CMAKE_SOURCE_DIR}/tests/test-lexing2.lox.expected -
                  DEPENDS jlox
)