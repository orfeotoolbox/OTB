# Sources of non-templated classes.

PROJECT(libkml)

SET(libkml_VERSION_MAJOR "0")
SET(libkml_VERSION_MINOR "4")
SET(libkml_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the library.
SET(libkml_VERSION_STRING "${libkml_VERSION_MAJOR}.${libkml_VERSION_MINOR}.${libkml_VERSION_PATCH}")


# Allow sources in subdirectories to see the include files.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.1/include)

SET(libkmlbase_la_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/kml/base)
INCLUDE_DIRECTORIES(${libkmlbase_la_SOURCES_DIR})
SET(libkmlbase_la_SOURCES 
	${libkmlbase_la_SOURCES_DIR}/file.cc 
	${libkmlbase_la_SOURCES_DIR}/referent.cc 
	${libkmlbase_la_SOURCES_DIR}/string_util.cc 
	${libkmlbase_la_SOURCES_DIR}/time_util.cc
)
IF(UNIX)
        SET(libkmlbase_la_SOURCES ${libkmlbase_la_SOURCES} 
	${libkmlbase_la_SOURCES_DIR}/file_posix.cc )
ELSE(UNIX)
        SET(libkmlbase_la_SOURCES ${libkmlbase_la_SOURCES} 
	${libkmlbase_la_SOURCES_DIR}/file_win32.cc 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h )
ENDIF(UNIX)

SET(libkmlconvenience_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/convenience")
INCLUDE_DIRECTORIES(${libkmlconvenience_la_SOURCES_DIR})
SET(libkmlconvenience_la_SOURCES 
	${libkmlconvenience_la_SOURCES_DIR}/csv_file.cc 
	${libkmlconvenience_la_SOURCES_DIR}/convenience.cc 
	${libkmlconvenience_la_SOURCES_DIR}/feature_list.cc 
	${libkmlconvenience_la_SOURCES_DIR}/kmz_check_links.cc
)

SET(libkmldom_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/dom")
INCLUDE_DIRECTORIES(${libkmldom_la_SOURCES_DIR})
SET(libkmldom_la_SOURCES 
	${libkmldom_la_SOURCES_DIR}/abstractlatlonbox.cc 
	${libkmldom_la_SOURCES_DIR}/abstractview.cc 
	${libkmldom_la_SOURCES_DIR}/balloonstyle.cc 
	${libkmldom_la_SOURCES_DIR}/colorstyle.cc 
	${libkmldom_la_SOURCES_DIR}/container.cc 
	${libkmldom_la_SOURCES_DIR}/document.cc 
	${libkmldom_la_SOURCES_DIR}/element.cc 
	${libkmldom_la_SOURCES_DIR}/extendeddata.cc 
	${libkmldom_la_SOURCES_DIR}/feature.cc 
	${libkmldom_la_SOURCES_DIR}/folder.cc 
	${libkmldom_la_SOURCES_DIR}/geometry.cc 
	${libkmldom_la_SOURCES_DIR}/hotspot.cc 
	${libkmldom_la_SOURCES_DIR}/iconstyle.cc 
	${libkmldom_la_SOURCES_DIR}/kml_cast.cc 
	${libkmldom_la_SOURCES_DIR}/kml_factory.cc 
	${libkmldom_la_SOURCES_DIR}/kml.cc 
	${libkmldom_la_SOURCES_DIR}/link.cc 
	${libkmldom_la_SOURCES_DIR}/labelstyle.cc 
	${libkmldom_la_SOURCES_DIR}/linestyle.cc 
	${libkmldom_la_SOURCES_DIR}/liststyle.cc 
	${libkmldom_la_SOURCES_DIR}/model.cc 
	${libkmldom_la_SOURCES_DIR}/networklink.cc 
	${libkmldom_la_SOURCES_DIR}/networklinkcontrol.cc 
	${libkmldom_la_SOURCES_DIR}/object.cc 
	${libkmldom_la_SOURCES_DIR}/overlay.cc 
	${libkmldom_la_SOURCES_DIR}/placemark.cc 
	${libkmldom_la_SOURCES_DIR}/polystyle.cc 
	${libkmldom_la_SOURCES_DIR}/region.cc 
	${libkmldom_la_SOURCES_DIR}/schema.cc 
	${libkmldom_la_SOURCES_DIR}/snippet.cc 
	${libkmldom_la_SOURCES_DIR}/style.cc 
	${libkmldom_la_SOURCES_DIR}/stylemap.cc 
	${libkmldom_la_SOURCES_DIR}/styleselector.cc 
	${libkmldom_la_SOURCES_DIR}/substyle.cc 
	${libkmldom_la_SOURCES_DIR}/timeprimitive.cc 
	${libkmldom_la_SOURCES_DIR}/vec2.cc 
	${libkmldom_la_SOURCES_DIR}/kml_handler.cc 
	${libkmldom_la_SOURCES_DIR}/parser.cc 
	${libkmldom_la_SOURCES_DIR}/serializer.cc 
	${libkmldom_la_SOURCES_DIR}/xml_serializer.cc 
	${libkmldom_la_SOURCES_DIR}/xsd.cc
)

SET(libkmlengine_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/engine")
INCLUDE_DIRECTORIES(${libkmlengine_la_SOURCES_DIR})
SET(libkmlengine_la_SOURCES 
	${libkmlengine_la_SOURCES_DIR}/clone.cc 
	${libkmlengine_la_SOURCES_DIR}/entity_mapper.cc 
	${libkmlengine_la_SOURCES_DIR}/feature_balloon.cc 
	${libkmlengine_la_SOURCES_DIR}/feature_visitor.cc 
	${libkmlengine_la_SOURCES_DIR}/find.cc 
	${libkmlengine_la_SOURCES_DIR}/get_links.cc 
	${libkmlengine_la_SOURCES_DIR}/get_link_parents.cc 
	${libkmlengine_la_SOURCES_DIR}/href.cc 
	${libkmlengine_la_SOURCES_DIR}/kml_cache.cc 
	${libkmlengine_la_SOURCES_DIR}/kml_file.cc 
	${libkmlengine_la_SOURCES_DIR}/kml_uri.cc 
	${libkmlengine_la_SOURCES_DIR}/kmz_cache.cc 
	${libkmlengine_la_SOURCES_DIR}/kmz_file.cc 
	${libkmlengine_la_SOURCES_DIR}/location_util.cc 
	${libkmlengine_la_SOURCES_DIR}/merge.cc 
	${libkmlengine_la_SOURCES_DIR}/style_merger.cc 
	${libkmlengine_la_SOURCES_DIR}/style_resolver.cc
)

SET(libkmlregionator_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/regionator")
INCLUDE_DIRECTORIES(${libkmlregionator_la_SOURCES_DIR})
SET(libkmlregionator_la_SOURCES 
	${libkmlregionator_la_SOURCES_DIR}/feature_list_region_handler.cc 
	${libkmlregionator_la_SOURCES_DIR}/regionator.cc 
	${libkmlregionator_la_SOURCES_DIR}/regionator_util.cc
)


# Build libkml library
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib")
SET(libzlib_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib/minizip")
INCLUDE_DIRECTORIES(${libzlib_la_SOURCES_DIR})
SET(libzlib_la_SOURCES
	${libzlib_la_SOURCES_DIR}/unzip.c 
	${libzlib_la_SOURCES_DIR}/ioapi.c 
	${libzlib_la_SOURCES_DIR}/zip.c
)

SET(liburiparser_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.1/lib")
INCLUDE_DIRECTORIES(${liburiparser_la_SOURCES_DIR})
SET( liburiparser_la_SOURCES 
	${liburiparser_la_SOURCES_DIR}/UriCommon.c 
	${liburiparser_la_SOURCES_DIR}/UriCommon.h 
	${liburiparser_la_SOURCES_DIR}/UriCompare.c 
	${liburiparser_la_SOURCES_DIR}/UriEscape.c 
	${liburiparser_la_SOURCES_DIR}/UriFile.c 
	${liburiparser_la_SOURCES_DIR}/UriIp4.c 
	${liburiparser_la_SOURCES_DIR}/UriIp4Base.c 
	${liburiparser_la_SOURCES_DIR}/UriIp4Base.h 
	${liburiparser_la_SOURCES_DIR}/UriNormalize.c 
	${liburiparser_la_SOURCES_DIR}/UriNormalizeBase.c 
	${liburiparser_la_SOURCES_DIR}/UriNormalizeBase.h 
	${liburiparser_la_SOURCES_DIR}/UriParse.c 
	${liburiparser_la_SOURCES_DIR}/UriParseBase.c 
	${liburiparser_la_SOURCES_DIR}/UriParseBase.h 
	${liburiparser_la_SOURCES_DIR}/UriQuery.c 
	${liburiparser_la_SOURCES_DIR}/UriRecompose.c 
	${liburiparser_la_SOURCES_DIR}/UriResolve.c 
	${liburiparser_la_SOURCES_DIR}/UriShorten.c
)

SET(libkml_SOURCES  
        ${libkmlbase_la_SOURCES} 
        ${libkmlconvenience_la_SOURCES} 
        ${libkmldom_la_SOURCES} 
        ${libkmlengine_la_SOURCES} 
        ${libkmlregionator_la_SOURCES} 
        ${libzlib_la_SOURCES} 
        ${liburiparser_la_SOURCES} 



        )
# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f ${libkml_SOURCES} )
#        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -Wall -Werror -ansi -pedantic -fno-rtti )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
        FOREACH(f ${libkml_SOURCES} )
                SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS /W0 )
        ENDFOREACH(f)
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
IF(WIN32)
        IF(NOT CYGWIN)
                ADD_DEFINITIONS(-DWIN32 -DUNICODE -D_UNICODE)
        ENDIF(NOT CYGWIN)
ENDIF(WIN32)

ADD_LIBRARY(otbkml ${libkml_SOURCES} )
IF( OTB_USE_EXTERNAL_EXPAT )
        TARGET_LINK_LIBRARIES(otbkml expat)
ELSE( OTB_USE_EXTERNAL_EXPAT  )
        TARGET_LINK_LIBRARIES(otbkml otbexpat)
ENDIF( OTB_USE_EXTERNAL_EXPAT )


INSTALL(TARGETS otbkml
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/dom/*.h")
INSTALL(FILES ${__files1} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml/dom
    COMPONENT Development)

FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/base/*.h")
INSTALL(FILES ${__files2} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml/base
    COMPONENT Development)

FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/convenience/*.h")
INSTALL(FILES ${__files3} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml/convenience
    COMPONENT Development)

FILE(GLOB __files4 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/engine/*.h")
INSTALL(FILES ${__files4} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml/engine
    COMPONENT Development)

FILE(GLOB __files5 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/regionator/*.h")
INSTALL(FILES ${__files5} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml/regionator
    COMPONENT Development)

FILE(GLOB __files6 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.1/include/uriparser/*.h")
INSTALL(FILES ${__files6} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/uriparser-0.7.1/include/uriparser
    COMPONENT Development)

FILE(GLOB __files70 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/*.h")
INSTALL(FILES ${__files70} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/boost_1_34_1/boost
    COMPONENT Development)
FILE(GLOB __files7 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/detail/*.h")
INSTALL(FILES ${__files7} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/boost_1_34_1/boost/detail
    COMPONENT Development)
FILE(GLOB __files8 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/*.h")
INSTALL(FILES ${__files8} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/boost_1_34_1/boost/config
    COMPONENT Development)
FILE(GLOB __files9 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/no_tr1/*.h")
INSTALL(FILES ${__files9} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/boost_1_34_1/boost/config/no_tr1
    COMPONENT Development)
FILE(GLOB __files10 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/stdlib/*.h")
INSTALL(FILES ${__files10} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/boost_1_34_1/boost/config/stdlib
    COMPONENT Development)

FILE(GLOB __files11 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/platform/*.h")
INSTALL(FILES ${__files11} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/boost_1_34_1/boost/config/platform
    COMPONENT Development)


FILE(GLOB __files12 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/*.h")
INSTALL(FILES ${__files12} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3
    COMPONENT Development)
FILE(GLOB __files13 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib/*.h")
INSTALL(FILES ${__files13} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3/contrib
    COMPONENT Development)
FILE(GLOB __files14 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib/minizip/*.h")
INSTALL(FILES ${__files14} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3/contrib/minizip
    COMPONENT Development)
