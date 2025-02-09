cmake_minimum_required(VERSION 3.30)
project(projet_image C)

set(CMAKE_C_STANDARD 99)

add_executable(projet_image main.c
        image.h
        image.c)
