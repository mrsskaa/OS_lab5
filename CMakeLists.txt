cmake_minimum_required(VERSION 3.16)
project(OS_lab5 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_compile_options(-Wall -Wextra)
include_directories(include)

add_library(contracts1 SHARED src/contracts1.c)
add_library(contracts2 SHARED src/contracts2.c)

add_executable(linked src/linked.c src/console.c)
target_link_libraries(linked PRIVATE contracts1)

add_executable(dynamic src/dynamic.c src/console.c)
target_link_libraries(dynamic PRIVATE ${CMAKE_DL_LIBS})
target_compile_definitions(dynamic PRIVATE
    LIBRARY_1="./$<TARGET_FILE_NAME:contracts1>"
    LIBRARY_2="./$<TARGET_FILE_NAME:contracts2>"
)
add_dependencies(dynamic contracts1 contracts2)
