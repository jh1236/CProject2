cmake_minimum_required(VERSION 3.22.1)
project(Project2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Project2 main.c
        main.h
        config.c
        config.h
        find_files.c
        find_files.h
        updateFiles.c
        updateFiles.h
        FileManagement.c
        FileManagement.h
        GlobToRegex.c
        GlobToRegex.h)
