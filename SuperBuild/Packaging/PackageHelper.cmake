#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

macro(macro_super_package)
  cmake_parse_arguments(PKG  "" "STAGE_DIR" "" ${ARGN} )
 
  set(LINUX FALSE)
  if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux" AND NOT MINGW)
    set(LINUX TRUE)
  endif()
  
  #setting this variable. prints a lot of debug information
  #set( PKG_DEBUG 1)

  set(loader_program_PATHS)
  set(eol_char "E")
  if(MSVC)
    set(loader_program_PATHS)
    set(loader_program_names      "dumpbin")
    set(loader_program_args       "/DEPENDENTS")
    set(loader_program_regex      "^    ([^ ].*[Dd][Ll][Ll])${eol_char}$")
  elseif(APPLE)
    set(loader_program_PATHS)
    set(loader_program_names    otool)
    set(loader_program_args      "-l" )
    set(loader_program_regex ".*name.*([ ].*dylib ).*${eol_char}$")
  elseif(LINUX)
    set(loader_program_PATHS)
    set(loader_program_names    objdump)
    set(loader_program_args    "-p" )
    set(loader_program_regex "^..NEEDED.*([ ].*so.*)${eol_char}$")
  else()
    message(FATAL_ERROR "Unknown platform: ")
  endif()
  
  find_program(LOADER_PROGRAM "${loader_program_names}" PATHS ${loader_program_PATHS})
  if(NOT LOADER_PROGRAM)
    message(FATAL_ERROR "${loader_program_names} not found in ${loader_program_PATHS}.")
  endif()
  

  include(GetPrerequisites)

  set(LOADER_PROGRAM_ARGS ${loader_program_args})

  set(DEST_BIN_DIR bin)
  set(DEST_APP_DIR lib/otb/applications)
  
  set(LIB_PREFIX lib)
  set(DEST_LIB_DIR lib)
  set(EXE_EXT "")
  set(SCRIPT_EXT ".sh")
  set(LIB_EXT ".so")
  set(PYMODULE_EXT ".so")
  if(WIN32)
    set(LIB_PREFIX)
    set(DEST_LIB_DIR bin)
    set(EXE_EXT ".exe")
    set(LIB_EXT ".dll")
    set(SCRIPT_EXT ".bat")
    set(PYMODULE_EXT ".pyd")
  elseif(APPLE)
    set(LIB_EXT ".dylib")
  endif()


  foreach(req
      PATCHELF_PROGRAM
      PKG_STAGE_DIR
      PACKAGE_SUPPORT_FILES_DIR
      SUPERBUILD_INSTALL_DIR
      SUPERBUILD_BINARY_DIR
      QT_TRANSLATIONS_DIR
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req}. Aborting.")
    endif()
  endforeach(req)

  set(HAVE_QT4 FALSE CACHE INTERNAL "HAVE_QT4")
  if(EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/otbApplicationLauncherQt${EXE_EXT}")
    set(HAVE_QT4 TRUE)
  endif()

  set(HAVE_MVD FALSE  CACHE INTERNAL "HAVE_MVD")
  if(EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/monteverdi${EXE_EXT}")
    set(HAVE_MVD TRUE)
  endif()

  set(HAVE_PYTHON FALSE CACHE INTERNAL "HAVE_PYTHON")
  if(EXISTS "${SUPERBUILD_INSTALL_DIR}/lib/otb/python/_otbApplication${PYMODULE_EXT}")
    set(HAVE_PYTHON TRUE)
  endif()

  # find_loader_and_args(LOADER_PROGRAM LOADER_PROGRAM_ARGS)
  
  if(HAVE_PYTHON)
    find_python_soname(python_INSTALLED_SONAME)
  endif()
  
  set(PKG_SEARCHDIRS)

  if(MSVC)
    if(DEFINED ENV{UniversalCRTSdkDir})
      message(FATAL_ERROR "UniversalCRTSdkDir variable not set. Cannot continue")
    endif()
    if(NOT DEFINED ENV{VCINSTALLDIR})
      message(FATAL_ERROR  "VCINSTALLDIR variable not set. Cannot continue.")
    endif()
  endif()

  if(MSVC)    
    file(TO_CMAKE_PATH "$ENV{UniversalCRTSdkDir}" UCRT_SDK_DIR)
    list(APPEND PKG_SEARCHDIRS "${UCRT_SDK_DIR}/Redist/ucrt/DLLs/${OTB_TARGET_SYSTEM_ARCH}") #ucrt dlls
    
    #additional msvc redist dll from VCINSTALLDIR
    file(TO_CMAKE_PATH "$ENV{VCINSTALLDIR}" PKG_VCINSTALLDIR)
    list(APPEND PKG_SEARCHDIRS 
      "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.CRT"
      "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.OPENMP"
      )
  endif()
  

  #superbuild .so /.dylib
  list(APPEND PKG_SEARCHDIRS "${SUPERBUILD_INSTALL_DIR}/lib")
  #all executables gdalinfo etc..
  list(APPEND PKG_SEARCHDIRS "${SUPERBUILD_INSTALL_DIR}/bin") 
  #common for all platforms.
  set(OTB_APPLICATIONS_DIR "${SUPERBUILD_INSTALL_DIR}/lib/otb/applications")
  list(APPEND PKG_SEARCHDIRS "${OTB_APPLICATIONS_DIR}")
  
  #_otbApplication.so
  list(APPEND PKG_SEARCHDIRS "${SUPERBUILD_INSTALL_DIR}/lib/otb/python")

  #for otbtest executables. 
  list(APPEND PKG_SEARCHDIRS ${OTB_BINARY_DIR}/bin)

  macro_empty_package_staging_directory()

  #only for *nix
  file(WRITE ${CMAKE_BINARY_DIR}/make_symlinks   "#!/bin/sh\n")


  set(program_list)

  if(LINUX)
    configure_file(
      ${PACKAGE_SUPPORT_FILES_DIR}/linux_pkgsetup.in
      ${CMAKE_CURRENT_BINARY_DIR}/pkgsetup @ONLY
      )
  endif()
  
  if(APPLE)
    configure_file(
      ${PACKAGE_SUPPORT_FILES_DIR}/macx_pkgsetup.in
      ${CMAKE_CURRENT_BINARY_DIR}/pkgsetup @ONLY
      )
  endif()


  ############# otb_loader executable ################
  add_executable(otb_loader ${PACKAGE_SUPPORT_FILES_DIR}/otb_loader.cxx)
  target_link_libraries(otb_loader ${CMAKE_DL_LIBS})
  install(TARGETS otb_loader DESTINATION ${PKG_STAGE_DIR}/bin)


  set(PKG_PEFILES)
  

  prepare_file_list(PKG_PEFILES)
#  func_prepare_package()


  unset(matched_vars CACHE)
  get_vars_ending_with("_USED|_RESOLVED" matched_vars)
  foreach (var_to_unset IN LISTS matched_vars)
    if(PKG_DEBUG)
      message("unset ${var_to_unset} from cache")
    endif()
    unset(${var_to_unset} CACHE)
  endforeach()

  foreach(infile ${PKG_PEFILES})
    get_filename_component(bn ${infile} NAME)
    process_file_recurse(${bn})
  endforeach()
  
  if(HAVE_PYTHON)
    install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/lib/otb/python
      DESTINATION ${PKG_STAGE_DIR}/lib
      )
  endif()


 install_include_dirs()
 
 install_share_dirs()
 
 install_translation_files()

 func_install_xdk_files()

  if(HAVE_MVD)
    func_install_monteverdi_support_files()
  endif()

  install_script_files()
  
  if(WIN32)
    install( PROGRAMS "${SUPERBUILD_INSTALL_DIR}/bin/otbcli.bat" DESTINATION ${PKG_STAGE_DIR}/bin)
    install( PROGRAMS "${SUPERBUILD_INSTALL_DIR}/bin/otbgui.bat" DESTINATION ${PKG_STAGE_DIR}/bin)
  else()
    install( PROGRAMS "${SUPERBUILD_INSTALL_DIR}/bin/otbcli" DESTINATION ${PKG_STAGE_DIR}/bin)
    install( PROGRAMS "${SUPERBUILD_INSTALL_DIR}/bin/otbgui" DESTINATION ${PKG_STAGE_DIR}/bin)
  endif()
  
   if(MSVC)
    install(FILES
      "${PACKAGE_SUPPORT_FILES_DIR}/OTB Project.zip"
      "${PACKAGE_SUPPORT_FILES_DIR}/start_devenv.bat"
      DESTINATION "${PKG_STAGE_DIR}" )
  endif()

  # We need qt.conf on windows. for macx and linux we write it
  # after extracting package
  if(WIN32)
    install(FILES
      ${PACKAGE_SUPPORT_FILES_DIR}/qt.conf
      DESTINATION ${PKG_STAGE_DIR}/bin
      )
  endif()
  
  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README
    DESTINATION ${PKG_STAGE_DIR}
    )

 
  ############# install package configure script ################
  if(UNIX)
    if(APPLE)
      set(ORIGINAL_RPATH_TO_REPLACE ${SUPERBUILD_INSTALL_DIR}/lib)
    endif()
        
    list(APPEND program_list "${CMAKE_BINARY_DIR}/pkgsetup")
    list(APPEND program_list "${CMAKE_BINARY_DIR}/make_symlinks")
 
    ########### install patchelf( linux only) ##################
    if(LINUX)
      list(APPEND program_list "${PATCHELF_PROGRAM}")
    endif()
  endif()

  
  foreach(prog ${program_list})
    install( PROGRAMS ${prog} DESTINATION ${PKG_STAGE_DIR})
  endforeach()


endmacro(macro_super_package)



