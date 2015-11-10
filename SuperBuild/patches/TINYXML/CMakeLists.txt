cmake_minimum_required(VERSION 2.8.3)

project(tinyXMLlib)

set(BUILD_SHARED_LIBS ON)
if(MSVC)
set(BUILD_SHARED_LIBS OFF) #force off
endif()


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


#set(TINYXML_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
#set(TINYXML_LIBRARIES tinyXML)
