cmake_minimum_required(VERSION 2.8.3)

project(muparser)


file(GLOB MUPARSER_SRCS "src/*.cpp" )
list(REMOVE_ITEM MUPARSER_SRCS "${CMAKE_SOURCE_DIR}/src/muParserComplex.cpp")

add_library(muparser ${MUPARSER_SRCS})
#if(OTB_LIBRARY_PROPERTIES)
#  set_target_properties(muparser PROPERTIES ${OTB_LIBRARY_PROPERTIES})
#endif()

install(TARGETS muparser
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib COMPONENT Development)



file(GLOB MUPARSER_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
list(REMOVE_ITEM MUPARSER_HEADERS "${CMAKE_SOURCE_DIR}/include/muParserComplex.h")
install(FILES ${MUPARSER_HEADERS}
  DESTINATION include
  COMPONENT Development)

