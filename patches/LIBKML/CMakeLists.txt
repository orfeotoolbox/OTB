cmake_minimum_required(VERSION 2.8.3)

project(libkml)

set(libkml_VERSION_MAJOR "1")
set(libkml_VERSION_MINOR "2")
set(libkml_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the library.
set(libkml_VERSION_STRING "${libkml_VERSION_MAJOR}.${libkml_VERSION_MINOR}.${libkml_VERSION_PATCH}")


# Allow sources in subdirectories to see the include files.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

find_package(Boost 1.35.0)
mark_as_advanced(Boost_DIR)
if(Boost_FOUND)
  message(STATUS "  Found Boost version ${Boost_VERSION}")
else()
  message(FATAL_ERROR "Cannot build libkml project without boost library (>1.35.0)")
endif()
include_directories(${Boost_INCLUDE_DIR})

find_package(EXPAT)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/include/uriparser)

set(libkmlbase_la_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/kml/base)
include_directories(${libkmlbase_la_SOURCES_DIR})
set(libkmlbase_la_SOURCES
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
if(UNIX)
        set(libkmlbase_la_SOURCES ${libkmlbase_la_SOURCES}
	${libkmlbase_la_SOURCES_DIR}/file_posix.cc )
else()
        set(libkmlbase_la_SOURCES ${libkmlbase_la_SOURCES}
	${libkmlbase_la_SOURCES_DIR}/file_win32.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h )
endif()

set(libkmlconvenience_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/convenience")
include_directories(${libkmlconvenience_la_SOURCES_DIR})
set(libkmlconvenience_la_SOURCES
	${libkmlconvenience_la_SOURCES_DIR}/csv_file.cc
	${libkmlconvenience_la_SOURCES_DIR}/convenience.cc
	${libkmlconvenience_la_SOURCES_DIR}/feature_list.cc
	${libkmlconvenience_la_SOURCES_DIR}/kmz_check_links.cc
)

set(libkmldom_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/dom")
include_directories(${libkmldom_la_SOURCES_DIR})
set(libkmldom_la_SOURCES
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

set(libkmlengine_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/engine")
include_directories(${libkmlengine_la_SOURCES_DIR})
set(libkmlengine_la_SOURCES
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

set(libkmlregionator_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/regionator")
include_directories(${libkmlregionator_la_SOURCES_DIR})
set(libkmlregionator_la_SOURCES
	${libkmlregionator_la_SOURCES_DIR}/feature_list_region_handler.cc
	${libkmlregionator_la_SOURCES_DIR}/regionator.cc
	${libkmlregionator_la_SOURCES_DIR}/regionator_util.cc
)


# Build libkml library
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib")
set(libzlib_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib/minizip")
include_directories(${libzlib_la_SOURCES_DIR})
set(libzlib_la_SOURCES
	${libzlib_la_SOURCES_DIR}/unzip.c
	${libzlib_la_SOURCES_DIR}/ioapi.c
	${libzlib_la_SOURCES_DIR}/zip.c
  ${libzlib_la_SOURCES_DIR}/iomem_simple.c
)
set(LIBKML_COMPILE_ZLIB OFF)


# Only for Windows Visual Studio and Mac OSX
set(libzlib_src_la_SOURCES "")
if(WIN32)
  if(NOT BORLAND)
#    if(NOT CYGWIN)
        include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        set(libzlib_src_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        set(libzlib_src_la_SOURCES
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
         set(LIBKML_COMPILE_ZLIB ON)
#      endif()
  endif()
endif()
if(APPLE)
        include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        set(libzlib_src_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3.src")
        set(libzlib_src_la_SOURCES
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
         set(LIBKML_COMPILE_ZLIB ON)
endif()


set(liburiparser_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/lib")
include_directories(${liburiparser_la_SOURCES_DIR})
set( liburiparser_la_SOURCES
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

set(libkml_SOURCES
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
if(CMAKE_COMPILER_IS_GNUCXX)
  foreach(f ${libkml_SOURCES} )
#        set_source_files_properties( ${f} PROPERTIES COMPILE_FLAGS -Wall -Werror -ansi -pedantic -fno-rtti )
        set_source_files_properties( ${f} PROPERTIES COMPILE_FLAGS -w )
  endforeach()
else()
  if(WIN32)
    foreach(f ${libkml_SOURCES} )
      set_source_files_properties( ${f} PROPERTIES COMPILE_FLAGS /W0 )
    endforeach()
  endif()
endif()
if(WIN32)
        if(NOT CYGWIN)
                add_definitions(-DWIN32 -DUNICODE -D_UNICODE)
        endif()
endif()

add_library(kml ${libkml_SOURCES} )
target_link_libraries(kml ${EXPAT_LIBRARIES})

if(NOT LIBKML_COMPILE_ZLIB)
  target_link_libraries(kml ${ZLIB_LIBRARY})
endif()

install(TARGETS kml
    RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
    LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION lib COMPONENT Development)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/kml
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

