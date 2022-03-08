cmake_minimum_required(VERSION 3.20)
project(m1s2_superpixel)

set(CMAKE_CXX_STANDARD 14)

include_directories(..)

add_executable(m1s2_superpixel
        color.h
        image.h
        pixel.h
        slic.cpp)
