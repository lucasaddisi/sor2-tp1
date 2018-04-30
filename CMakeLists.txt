cmake_minimum_required(VERSION 3.10)
project(sor2_tp1 C)

set(CMAKE_C_STANDARD 11)

add_executable(sor2_tp1
        documentacion/read_boot.c
        documentacion/read_mbr.c
        documentacion/read_root.c
        readFile.c)
