cmake_minimum_required(VERSION 2.8.3)

project(tinyXMLlib)

option(BUILD_SHARED_LIBS "Building shared libs. Default is ON" ON)

add_definitions(-DTINYXML_USE_STL=1)

set(tinyXMLlib_SRCS
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp)

set(tinyXMLlib_HDRS
  tinystr.h
  tinyxml.h)

add_library(tinyxml ${tinyXMLlib_SRCS})

install(TARGETS tinyxml
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(FILES ${tinyXMLlib_HDRS}
  DESTINATION include)
