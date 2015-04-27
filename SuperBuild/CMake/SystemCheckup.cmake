# Script to analyse the OTB dependencies already present

message(STATUS "|----------------------------------------------------------")
message(STATUS "|----------------- System checkup results -----------------")
message(STATUS "| The following libraries have been found on your system.")
message(STATUS "| You can choose to use them (instead of superbuild versions)")
message(STATUS "| by setting the corresponding option USE_SYSTEM_XXX.")

# try to hide the Superbuild install dir, CMAKE_SYSTEM_PREFIX_PATH
# contains the CMAKE_INSTALL_PREFIX (-> superbuild libs)
list(REMOVE_ITEM CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})

# macro to encapsulate a find_package(XXX QUIET)
# output variables are :
#   _SB_CHECKUP_XXX_FOUND           : lib is found ?
#   _SB_CHECKUP_XXX_VERSION         : version found
#   _SB_CHECKUP_XXX_NAME            : name found
macro(SB_CHECKUP_FIND_PACKAGE var)
  find_package(${var} QUIET)
  string(TOUPPER ${var} _uppervar)
  set(_SB_CHECKUP_${_uppervar}_VERSION NOTFOUND)
  set(_SB_CHECKUP_${_uppervar}_FOUND FALSE)
  set(_SB_CHECKUP_${_uppervar}_NAME NOTFOUND)
  if(${_uppervar}_FOUND)
    set(_SB_CHECKUP_${_uppervar}_FOUND TRUE)
    set(_SB_CHECKUP_${_uppervar}_NAME ${_uppervar})
  elseif(${var}_FOUND)
    set(_SB_CHECKUP_${_uppervar}_FOUND TRUE)
    set(_SB_CHECKUP_${_uppervar}_NAME ${var})
  endif()
  
  if(_SB_CHECKUP_${_uppervar}_FOUND)
    set(_var_name ${_SB_CHECKUP_${_uppervar}_NAME})
    message(STATUS "|----------------------------------------------------------")
    message(STATUS "|${var} found")
    if(DEFINED ${_var_name}_VERSION)
      set(_SB_CHECKUP_${_uppervar}_VERSION ${${_var_name}_VERSION})
    elseif(DEFINED ${_var_name}_VERSION_STRING)
      set(_SB_CHECKUP_${_uppervar}_VERSION ${${_var_name}_VERSION_STRING})
    elseif(DEFINED ${_var_name}_VERSION_MAJOR AND DEFINED ${_var_name}_VERSION_MINOR AND DEFINED ${_var_name}_VERSION_PATCH)
      set(_SB_CHECKUP_${_uppervar}_VERSION "${${_var_name}_VERSION_MAJOR}.${${_var_name}_VERSION_MINOR}.${${_var_name}_VERSION_PATCH}")
    elseif(DEFINED ${_var_name}_MAJOR_VERSION AND DEFINED ${_var_name}_MINOR_VERSION AND DEFINED ${_var_name}_BUILD_VERSION)
      set(_SB_CHECKUP_${_uppervar}_VERSION "${${_var_name}_MAJOR_VERSION}.${${_var_name}_MINOR_VERSION}.${${_var_name}_BUILD_VERSION}")
    endif()
    
    # fix incomplete version numbers
    if(_SB_CHECKUP_${_uppervar}_VERSION MATCHES "^[0-9]+\\.[0-9]+\$")
      set(_SB_CHECKUP_${_uppervar}_VERSION "${_SB_CHECKUP_${_uppervar}_VERSION}.0")
    endif()
    # display version number
    if(_SB_CHECKUP_${_uppervar}_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+\$")
      message(STATUS "|  version = ${_SB_CHECKUP_${_uppervar}_VERSION}")
    endif()
    
  endif()
endmacro(SB_CHECKUP_FIND_PACKAGE)

macro(SB_CHECKUP_SPLIT_VERSION var)
  string(TOUPPER ${var} _upperlib)
  if(_SB_CHECKUP_${_upperlib}_VERSION)
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1"
           _SB_CHECKUP_${_upperlib}_VERSION_MAJOR
           ${_SB_CHECKUP_${_upperlib}_VERSION})
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2"
           _SB_CHECKUP_${_upperlib}_VERSION_MINOR
           ${_SB_CHECKUP_${_upperlib}_VERSION})
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3"
           _SB_CHECKUP_${_upperlib}_VERSION_PATCH
           ${_SB_CHECKUP_${_upperlib}_VERSION})
    math(EXPR _SB_CHECKUP_${_upperlib}_VERSION_NUMBER
      "((${_SB_CHECKUP_${_upperlib}_VERSION_MAJOR})*100+${_SB_CHECKUP_${_upperlib}_VERSION_MINOR})*100+${_SB_CHECKUP_${_upperlib}_VERSION_PATCH}")
  endif()
endmacro(SB_CHECKUP_SPLIT_VERSION)

macro(SB_CHECKUP_FIND_VERSIONS lib header major_name minor_name patch_name)
  string(TOUPPER ${lib} _upperlib)
  if(_SB_CHECKUP_${_upperlib}_FOUND)
    foreach(inc_path ${${_upperlib}_INCLUDE_DIRS})
      if(EXISTS "${inc_path}/${header}")
        unset(_header_content)
        set(_major_regex ".*# *define +${major_name} +([0-9]+).*")
        set(_minor_regex ".*# *define +${minor_name} +([0-9]+).*")
        set(_patch_regex ".*# *define +${patch_name} +([0-9]+).*")
        file(READ "${inc_path}/${header}" _header_content)
        string(REGEX REPLACE ${_major_regex} "\\1"
               _SB_CHECKUP_${_upperlib}_VERSION_MAJOR
               ${_header_content})
        string(REGEX REPLACE ${_minor_regex} "\\1"
               _SB_CHECKUP_${_upperlib}_VERSION_MINOR
               ${_header_content})
        string(REGEX REPLACE ${_patch_regex} "\\1"
               _SB_CHECKUP_${_upperlib}_VERSION_PATCH
               ${_header_content})
        math(EXPR _SB_CHECKUP_${_upperlib}_VERSION_NUMBER
          "((${_SB_CHECKUP_${_upperlib}_VERSION_MAJOR})*100+${_SB_CHECKUP_${_upperlib}_VERSION_MINOR})*100+${_SB_CHECKUP_${_upperlib}_VERSION_PATCH}")
        set(_SB_CHECKUP_${_upperlib}_VERSION "${_SB_CHECKUP_${_upperlib}_VERSION_MAJOR}.${_SB_CHECKUP_${_upperlib}_VERSION_MINOR}.${_SB_CHECKUP_${_upperlib}_VERSION_PATCH}")
        message(STATUS "|  version = ${_SB_CHECKUP_${_upperlib}_VERSION}")
        break()
      endif()
    endforeach()
  endif()
endmacro(SB_CHECKUP_FIND_VERSIONS)

# Boost
SB_CHECKUP_FIND_PACKAGE(Boost)
if(_SB_CHECKUP_BOOST_VERSION MATCHES "^[0-9]+\$")
  set(_SB_CHECKUP_BOOST_VERSION_NUMBER ${_SB_CHECKUP_BOOST_VERSION})
  math(EXPR _SB_CHECKUP_BOOST_VERSION_PATCH "(${_SB_CHECKUP_BOOST_VERSION_NUMBER})%100")
  math(EXPR _SB_CHECKUP_BOOST_VERSION_MINOR "((${_SB_CHECKUP_BOOST_VERSION_NUMBER})/100)%1000")
  math(EXPR _SB_CHECKUP_BOOST_VERSION_MAJOR "(${_SB_CHECKUP_BOOST_VERSION_NUMBER})/100000")
  set(_SB_CHECKUP_BOOST_VERSION "${_SB_CHECKUP_BOOST_VERSION_MAJOR}.${_SB_CHECKUP_BOOST_VERSION_MINOR}.${_SB_CHECKUP_BOOST_VERSION_PATCH}")
  message(STATUS "|  version = ${_SB_CHECKUP_BOOST_VERSION}")
endif()

# Curl
SB_CHECKUP_FIND_PACKAGE(CURL)
SB_CHECKUP_SPLIT_VERSION(CURL)

# GDAL
SB_CHECKUP_FIND_PACKAGE(GDAL)
if(NOT _SB_CHECKUP_GDAL_VERSION)
  SB_CHECKUP_FIND_VERSIONS(GDAL 
                           gdal_version.h
                           GDAL_VERSION_MAJOR
                           GDAL_VERSION_MINOR
                           GDAL_VERSION_REV)
endif()

# GeoTIFF
SB_CHECKUP_FIND_PACKAGE(GeoTIFF)

# ITK
SB_CHECKUP_FIND_PACKAGE(ITK)

# libkml
SB_CHECKUP_FIND_PACKAGE(LibKML)
if(NOT _SB_CHECKUP_LIBKML_VERSION)
  SB_CHECKUP_FIND_VERSIONS(LibKML 
                           kml/base/version.h
                           LIBKML_MAJOR_VERSION
                           LIBKML_MINOR_VERSION
                           LIBKML_MICRO_VERSION)
endif()

# LibSVM
SB_CHECKUP_FIND_PACKAGE(LibSVM)
SB_CHECKUP_SPLIT_VERSION(LibSVM)

# muParser
SB_CHECKUP_FIND_PACKAGE(MuParser)

# muParserX
SB_CHECKUP_FIND_PACKAGE(MuParserX)

# OpenCV
SB_CHECKUP_FIND_PACKAGE(OpenCV)
SB_CHECKUP_SPLIT_VERSION(OpenCV)

# OpenJPEG
SB_CHECKUP_FIND_PACKAGE(OpenJPEG)
if(NOT _SB_CHECKUP_OPENJPEG_VERSION)
  SB_CHECKUP_FIND_VERSIONS(OpenJPEG
                           opj_config.h
                           OPJ_VERSION_MAJOR
                           OPJ_VERSION_MINOR
                           OPJ_VERSION_BUILD)
endif()

# OpenThreads
SB_CHECKUP_FIND_PACKAGE(OpenThreads)

# OSSIM
SB_CHECKUP_FIND_PACKAGE(Ossim)

# Qt4
SB_CHECKUP_FIND_PACKAGE(Qt4)
if(_SB_CHECKUP_QT4_FOUND)
  set(_SB_CHECKUP_QT4_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
  message(STATUS "|  version = ${_SB_CHECKUP_QT4_VERSION}")
  SB_CHECKUP_SPLIT_VERSION(Qt4)
endif()

# TinyXML
SB_CHECKUP_FIND_PACKAGE(TinyXML)

message(STATUS "|----------------------------------------------------------")
# restore real install prefix
list(APPEND CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
