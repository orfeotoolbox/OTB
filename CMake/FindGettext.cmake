# - Find GNU gettext tools
# This module looks for the GNU gettext tools. This module defines the
# following values:
#  GETTEXT_MSGMERGE_EXECUTABLE: the full path to the msgmerge tool.
#  GETTEXT_MSGFMT_EXECUTABLE: the full path to the msgfmt tool.
#  GETTEXT_FOUND: True if gettext has been found.
#
# Additionally it provides the following macros:
# GETTEXT_CREATE_TRANSLATIONS ( outputFile [ALL] file1 ... fileN )
#    This will create a target "translations" which will convert the
#    given input po files into the binary output mo file. If the
#    ALL option is used, the translations will also be created when
#    building the default target.

#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_program(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)

find_program(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

macro(GETTEXT_CREATE_TRANSLATIONS _potFile _firstPoFileArg)
   # make it a real variable, so we can modify it here
   set(_firstPoFile "${_firstPoFileArg}")

   set(_gmoFiles)
   get_filename_component(_potBasename ${_potFile} NAME_WE)
   get_filename_component(_absPotFile ${_potFile} ABSOLUTE)

   set(_addToAll)
   if(${_firstPoFile} STREQUAL "ALL")
      set(_addToAll "ALL")
      set(_firstPoFile)
   endif()

   foreach(_currentPoFile ${_firstPoFile} ${ARGN})
      get_filename_component(_absFile ${_currentPoFile} ABSOLUTE)
      get_filename_component(_abs_PATH ${_absFile} PATH)
      get_filename_component(_lang ${_absFile} NAME_WE)
      set(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)

      add_custom_command(
         OUTPUT ${_gmoFile}
         COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --quiet --update --backup=none -s ${_absFile} ${_absPotFile}
         COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_absFile}
         DEPENDS ${_absPotFile} ${_absFile}
      )

      install(FILES ${_gmoFile} DESTINATION share/locale/${_lang}/LC_MESSAGES RENAME ${_potBasename}.mo)
      set(_gmoFiles ${_gmoFiles} ${_gmoFile})

   endforeach()

   add_custom_target(translations ${_addToAll} DEPENDS ${_gmoFiles})

endmacro()

if(GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )
   set(GETTEXT_FOUND TRUE)
else()
   set(GETTEXT_FOUND FALSE)
   if(GetText_REQUIRED)
      message(FATAL_ERROR "GetText not found")
   endif()
endif()



