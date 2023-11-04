
cmake_minimum_required(VERSION 3.10)
project(jlox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

add_executable(jlox lox.cc)
add_executable(ast_printer ast_printer_driver.cc)

# add_custom_command(
#     OUTPUT expr.h
#     COMMAND ${CMAKE_BINARY_DIR}/generate_ast .
#     DEPENDS generate_ast
# )

# set_source_files_properties(lox.cc PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/expr.h")
set_source_files_properties(ast_printer_driver.cc PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/expr.h")


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