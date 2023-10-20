cmake_minimum_required(VERSION 3.22.1)
project(mysync C)

set(CMAKE_C_STANDARD 11)

add_executable(mysync main.c
        main.h
        Config.c
        Config.h
        FindFiles.c
        FindFiles.h
        FileManagement.c
        FileManagement.h
        GlobToRegex.c
        GlobToRegex.h)
