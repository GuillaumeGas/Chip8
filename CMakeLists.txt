CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(Chip8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb sdl-config --cflags --libs")

file (
  GLOB_RECURSE
  source_files
  src/*
)

add_executable (
  ../chip8
  main.cpp
  ${source_files}
)