cmake_minimum_required(VERSION 3.10)
project(sor2_tp1 C)

set(CMAKE_C_STANDARD 11)

add_executable(sor2_tp1
        read_boot.c
        read_mbr.c
        read_root.c
        readFile.c)
