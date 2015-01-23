cmake_minimum_required(VERSION 2.8.3)

project(muparserx)

set(BUILD_SHARED_LIBS ON)
if(MSVC)
set(BUILD_SHARED_LIBS OFF) #force off
endif()

file(GLOB MUPARSERX_SRCS "parser/*.cpp" )
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/parser")

add_library(muparserx ${MUPARSERX_SRCS})

install(TARGETS muparserx
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib COMPONENT Development)

file(GLOB MUPARSERX_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/parser/*.h")
install(FILES ${MUPARSERX_HEADERS}
  DESTINATION include
  COMPONENT Development)
