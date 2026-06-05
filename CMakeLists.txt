cmake_minimum_required(VERSION 3.16)

project(oop_lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(oop_lab2 main.cpp
    set.h
    setexception.h
    iterator.h
    iteratorexception.h
    tests.h
    tests.cpp
)

include(GNUInstallDirs)
install(TARGETS oop_lab2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
