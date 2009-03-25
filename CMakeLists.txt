
cmake_minimum_required(VERSION 2.6)

PROJECT(mapnik)

INCLUDE_DIRECTORIES("/home/christop/slash/include" )
INCLUDE_DIRECTORIES("/usr/include/freetype2" )
LINK_DIRECTORIES("/home/christop/slash/lib")

ADD_EXECUTABLE(myOwnMapnik myOwnMapnik.cxx)
TARGET_LINK_LIBRARIES(myOwnMapnik mapnik)