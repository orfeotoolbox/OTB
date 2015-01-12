project(tinyXMLlib)

file(GLOB tinyXMLlib_SRCS "*.cpp")
file(GLOB tinyXMLlib_HDRS "*.h")
add_library(tinyXML ${tinyXMLlib_SRCS})

install(TARGETS tinyXML
    RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
    LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION lib COMPONENT Development)

install(FILES ${tinyXMLlib_HDRS}
    DESTINATION include
    COMPONENT Development)


#set(TINYXML_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
#set(TINYXML_LIBRARIES tinyXML)
