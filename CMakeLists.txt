cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(makina)

add_subdirectory(./library)

add_subdirectory(./examples/simple)
set_property(TARGET makina_simple_example PROPERTY FOLDER "Examples")
