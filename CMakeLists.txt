cmake_minimum_required(VERSION 3.7)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Instrukcja.cpp Instrukcja.h Int.cpp Int.h KodProgramu.cpp KodProgramu.h KodWhileIf.cpp KodWhileIf.h Stos.cpp Stos.h Stos_Int.cpp Stos_Int.h StosElement.cpp StosElement.h StosElement_Int.cpp StosElement_Int.h WhileIf.cpp WhileIf.h)
add_executable(untitled ${SOURCE_FILES})