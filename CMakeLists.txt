cmake_minimum_required(VERSION 3.22)
project(f_module LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp)


add_library(my_module SHARED f_module.cpp)

# Enable Limited API
target_compile_definitions(my_module PRIVATE Py_LIMITED_API=0x03070000)
target_compile_definitions(my_module PRIVATE PY_SSIZE_T_CLEAN)

# Link to Python
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
target_include_directories(my_module PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(my_module PRIVATE ${Python3_LIBRARIES})

# Use .so instead of lib prefix
set_target_properties(my_module PROPERTIES PREFIX "")




set(MODULE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()

add_test(NAME PythonUnitTests
        COMMAND ${Python3_EXECUTABLE} -m unittest discover -s ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set_tests_properties(PythonUnitTests PROPERTIES
        ENVIRONMENT "PYTHONPATH=${MODULE_OUTPUT_DIR}"
)
