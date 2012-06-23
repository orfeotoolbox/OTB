# Sources of non-templated classes.

PROJECT(libkml)

SET(libkml_VERSION_MAJOR "1")
SET(libkml_VERSION_MINOR "2")
SET(libkml_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the library.
SET(libkml_VERSION_STRING "${libkml_VERSION_MAJOR}.${libkml_VERSION_MINOR}.${libkml_VERSION_PATCH}")


# Allow sources in subdirectories to see the include files.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party)
IF(NOT OTB_USE_EXTERNAL_BOOST)
  INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Utilities/BGL)
ELSE(NOT OTB_USE_EXTERNAL_BOOST)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF(NOT OTB_USE_EXTERNAL_BOOST)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/include/uriparser)

SET(libkmlbase_la_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/kml/base)
INCLUDE_DIRECTORIES(${libkmlbase_la_SOURCES_DIR})
SET(libkmlbase_la_SOURCES
	${libkmlbase_la_SOURCES_DIR}/file.cc
	${libkmlbase_la_SOURCES_DIR}/referent.cc
	${libkmlbase_la_SOURCES_DIR}/string_util.cc
	${libkmlbase_la_SOURCES_DIR}/time_util.cc
  ${libkmlbase_la_SOURCES_DIR}/attributes.cc
  ${libkmlbase_la_SOURCES_DIR}/uri_parser.cc
  ${libkmlbase_la_SOURCES_DIR}/expat_parser.cc
  ${libkmlbase_la_SOURCES_DIR}/math_util.cc
  ${libkmlbase_la_SOURCES_DIR}/zip_file.cc
  ${libkmlbase_la_SOURCES_DIR}/date_time.cc
  ${libkmlbase_la_SOURCES_DIR}/xml_namespaces.cc
  ${libkmlbase_la_SOURCES_DIR}/expat_handler_ns.cc
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
  ${libkmldom_la_SOURCES_DIR}/gx_tour.cc
  ${libkmldom_la_SOURCES_DIR}/gx_timeprimitive.cc
  ${libkmldom_la_SOURCES_DIR}/xal.cc
  ${libkmldom_la_SOURCES_DIR}/kml_handler_ns.cc
  ${libkmldom_la_SOURCES_DIR}/atom.cc
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
  ${libkmlengine_la_SOURCES_DIR}/find_xml_namespaces.cc
  ${libkmlengine_la_SOURCES_DIR}/id_mapper.cc
  ${libkmlengine_la_SOURCES_DIR}/feature_view.cc
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
  ${libzlib_la_SOURCES_DIR}/iomem_simple.c
)
SET(LIBKML_COMPILE_ZLIB OFF)


# Only for Windows Visual Studio and Mac OSX
SET(libzlib_src_la_SOURCES "")
IF(WIN32)
  IF(NOT BORLAND)
#    IF(NOT CYGWIN)
      IF(NOT MINGW)
        INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        SET(libzlib_src_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        SET(libzlib_src_la_SOURCES
                ${libzlib_src_la_SOURCES_DIR}/adler32.c
                ${libzlib_src_la_SOURCES_DIR}/compress.c
                ${libzlib_src_la_SOURCES_DIR}/crc32.c
                ${libzlib_src_la_SOURCES_DIR}/deflate.c
                ${libzlib_src_la_SOURCES_DIR}/inflate.c
                ${libzlib_src_la_SOURCES_DIR}/gzio.c
                ${libzlib_src_la_SOURCES_DIR}/infback.c
                ${libzlib_src_la_SOURCES_DIR}/inffast.c
                ${libzlib_src_la_SOURCES_DIR}/inftrees.c
                ${libzlib_src_la_SOURCES_DIR}/trees.c
                ${libzlib_src_la_SOURCES_DIR}/uncompr.c
                ${libzlib_src_la_SOURCES_DIR}/zutil.c )
         SET(LIBKML_COMPILE_ZLIB ON)
      ENDIF(NOT MINGW)
#    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)
IF(APPLE)
        INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        SET(libzlib_src_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        SET(libzlib_src_la_SOURCES
                ${libzlib_src_la_SOURCES_DIR}/adler32.c
                ${libzlib_src_la_SOURCES_DIR}/compress.c
                ${libzlib_src_la_SOURCES_DIR}/crc32.c
                ${libzlib_src_la_SOURCES_DIR}/deflate.c
                ${libzlib_src_la_SOURCES_DIR}/inflate.c
                ${libzlib_src_la_SOURCES_DIR}/gzio.c
                ${libzlib_src_la_SOURCES_DIR}/infback.c
                ${libzlib_src_la_SOURCES_DIR}/inffast.c
                ${libzlib_src_la_SOURCES_DIR}/inftrees.c
                ${libzlib_src_la_SOURCES_DIR}/trees.c
                ${libzlib_src_la_SOURCES_DIR}/uncompr.c
                ${libzlib_src_la_SOURCES_DIR}/zutil.c )
         SET(LIBKML_COMPILE_ZLIB ON)
ENDIF(APPLE)


SET(liburiparser_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/lib")
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
        ${libzlib_src_la_SOURCES}
        )
# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f ${libkml_SOURCES} )
#        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -Wall -Werror -ansi -pedantic -fno-rtti )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    FOREACH(f ${libkml_SOURCES} )
      SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS /W0 )
    ENDFOREACH(f)
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
IF(WIN32)
        IF(NOT CYGWIN)
                ADD_DEFINITIONS(-DWIN32 -DUNICODE -D_UNICODE)
        ENDIF(NOT CYGWIN)
ENDIF(WIN32)

ADD_LIBRARY(otbkml ${libkml_SOURCES} )
TARGET_LINK_LIBRARIES(otbkml ${EXPAT_LIBRARIES})

IF(NOT LIBKML_COMPILE_ZLIB)
  TARGET_LINK_LIBRARIES(otbkml ${ZLIB_LIBRARY})
ENDIF(NOT LIBKML_COMPILE_ZLIB)


IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(otbkml PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbkml
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/kml
          DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/
          FILES_MATCHING PATTERN "*.h"
          COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
