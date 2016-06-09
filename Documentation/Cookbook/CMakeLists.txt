cmake_minimum_required(VERSION 2.8)

# This is the Orfeo ToolBox cookbook : a guide for non-developpers
project(CookBook)

# need an OTB build dir
find_package(OTB REQUIRED)
find_package(PythonInterp REQUIRED)

add_subdirectory(rst)
