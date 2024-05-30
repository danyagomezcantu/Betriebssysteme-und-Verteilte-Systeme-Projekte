cmake_minimum_required(VERSION 3.10.2)
project(Betriebssysteme_und_Verteilte_Systeme_Projekte C)

set(CMAKE_C_STANDARD 99)

add_executable(Betriebssysteme_und_Verteilte_Systeme_Projekte main.c
        sub.c
        sub.h
        keyValStore.c
        keyValStore.h
        main.h)


find_package(Threads REQUIRED)
target_link_libraries(Betriebssysteme_und_Verteilte_Systeme_Projekte Threads::Threads)