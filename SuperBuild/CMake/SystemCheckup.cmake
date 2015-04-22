# Script to analyse the OTB dependencies already present

message(STATUS "|------------------------------------------------")
message(STATUS "|------------ System checkup results ------------")

# try to hide the Superbuild install dir, CMAKE_SYSTEM_PREFIX_PATH
# contains the CMAKE_INSTALL_PREFIX (-> superbuild libs)
list(REMOVE_ITEM CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})

macro(SB_BASIC_LIBRARY_CHECKUP var)
  find_package(${var} QUIET)
  string(TOUPPER ${var} _uppervar)
  unset(_SB_CHECKUP_RESULT_VERSION)
  set(_find_var_result 0)
  set(_var_name "")
  if(${var}_FOUND)
    set(_find_var_result 1)
    set(_var_name ${var})
  elseif(${_uppervar}_FOUND)
    set(_find_var_result 1)
    set(_var_name ${_uppervar})
  endif()
  
  if(_find_var_result)
    message(STATUS "|------------------------------------------------")
    message(STATUS "|${var} : found")
    if(DEFINED ${_var_name}_VERSION)
      message(STATUS "|${var} : version = ${${_var_name}_VERSION}")
      set(_SB_CHECKUP_RESULT_VERSION ${${_var_name}_VERSION})
    elseif(DEFINED ${_var_name}_VERSION_STRING)
      message(STATUS "|${var} : version = ${${_var_name}_VERSION_STRING}")
      set(_SB_CHECKUP_RESULT_VERSION ${${_var_name}_VERSION_STRING})
    elseif(DEFINED ${_var_name}_VERSION_MAJOR AND DEFINED ${_var_name}_VERSION_MINOR AND DEFINED ${_var_name}_VERSION_PATCH)
      message(STATUS "|${var} : version = ${${_var_name}_VERSION_MAJOR}.${${_var_name}_VERSION_MINOR}.${${_var_name}_VERSION_PATCH}")
      set(_SB_CHECKUP_RESULT_VERSION "${${_var_name}_VERSION_MAJOR}.${${_var_name}_VERSION_MINOR}.${${_var_name}_VERSION_PATCH}")
    endif()
    if(DEFINED ${_var_name}_DIR)
      message(STATUS "|${var} : directory = ${${_var_name}_DIR}")
    endif()
    if(DEFINED ${_var_name}_LIBRARIES)
      message(STATUS "|${var} : libraries = ${${_var_name}_LIBRARIES}")
    endif()
    if(DEFINED ${_var_name}_INCLUDE_DIRS)
      message(STATUS "|${var} : includes = ${${_var_name}_INCLUDE_DIRS}")
    endif()  
  endif()
endmacro(SB_BASIC_LIBRARY_CHECKUP)

# Boost
SB_BASIC_LIBRARY_CHECKUP(Boost)

# Curl
SB_BASIC_LIBRARY_CHECKUP(CURL)

# GDAL
SB_BASIC_LIBRARY_CHECKUP(GDAL)

# GeoTIFF
SB_BASIC_LIBRARY_CHECKUP(GeoTIFF)

# ITK
SB_BASIC_LIBRARY_CHECKUP(ITK)

# libkml
SB_BASIC_LIBRARY_CHECKUP(LibKML)

# LibSVM
SB_BASIC_LIBRARY_CHECKUP(LibSVM)

# muParser
SB_BASIC_LIBRARY_CHECKUP(MuParser)

# muParserX
SB_BASIC_LIBRARY_CHECKUP(MuParserX)

# OpenCV
SB_BASIC_LIBRARY_CHECKUP(OpenCV)

# OpenJPEG
SB_BASIC_LIBRARY_CHECKUP(OpenJPEG)

# OpenThreads
SB_BASIC_LIBRARY_CHECKUP(OpenThreads)

# OSSIM
SB_BASIC_LIBRARY_CHECKUP(Ossim)

# Qt4
SB_BASIC_LIBRARY_CHECKUP(Qt4)

# TinyXML
SB_BASIC_LIBRARY_CHECKUP(TinyXML)

message(STATUS "|------------------------------------------------")
# restore real install prefix
list(APPEND CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
