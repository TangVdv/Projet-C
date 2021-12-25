cmake_minimum_required(VERSION 3.20)
project(Projet-C C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-lsqlite3)


find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(Projet-C main.c)

target_link_libraries(Projet-C ${GTK3_LIBRARIES})