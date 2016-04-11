set(WINDOWS_SYSTEM_DLLS
  msvc.*dll
  user32.dll
  gdi32.dll
  shell32.dll
  kernel32.dll
  advapi32.dll
  crypt32.dll
  ws2_32.dll
  wldap32.dll
  ole32.dll
  opengl32.dll
  glu32.dll
  comdlg32.dll
  imm32.dll
  oleaut32.dll
  comctl32.dll
  winmm.dll
  shfolder.dll
  secur32.dll
  wsock32.dll
  winspool.drv)

SET(UNIX_SYSTEM_DLLS
  libm.so
  libc.so
  libstdc*
  libgcc_s.so
  librt.so
  libdl.so
  libpthread.so
  libidn.so
  libgomp.so*
  ld-linux-x86-64.so*
  libX11.so*
  libXext.so*
  libXau.so*
  libXdmcp.so*
  libXxf86vm.so*
  libdrm.so.2
  libGL.so*
  libGLU.so*
  )
set(SYSTEM_DLLS "${UNIX_SYSTEM_DLLS}")
if(WIN32 OR CMAKE_CROSSCOMPILING)
  set(SYSTEM_DLLS "${WINDOWS_SYSTEM_DLLS}")
endif()

## http://www.cmake.org/Wiki/CMakeMacroListOperations
macro(is_system_dll matched value)
  set(${matched})
  string(TOLOWER ${value} value_)
  foreach (pattern ${SYSTEM_DLLS})
    string(TOLOWER ${pattern} pattern_)
    if(${value_} MATCHES ${pattern_})
      set(${matched} TRUE)
    endif()
  endforeach()
endmacro()

macro(list_contains var value)
  set(${var})
  foreach(value2 ${ARGN})
    if(${value} STREQUAL ${value2})
      set(${var} TRUE)
    endif()
  endforeach(value2)
endmacro()

# Get the translation files coming with Qt, and install them in the bundle
# They are loaded by Monteverdi.
function(get_qt_translation_files RESULT)
    # These files are the "qt_<localename>.qm" files
    # They are located in QT_TRANSLATIONS_DIR, which comes from FindQt4
    file(GLOB translation_files ${QT_TRANSLATIONS_DIR}/qt_*)

    # We need to remove the "qt_help_<localename>.qm" files from this list
    foreach(translation_item ${translation_files})
      if(${translation_item} MATCHES "qt_help")
        list(REMOVE_ITEM translation_files ${translation_item})
      endif()
    endforeach()

    set(${RESULT} ${translation_files} PARENT_SCOPE)
endfunction()

function(install_common include_mvd)

  #a convenient cmake var for storing <prefix>
#  set(PKG_STAGE_DIR "${stage_dir}")

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_STAGE_BIN_DIR "${PKG_STAGE_DIR}/bin")

  #root folder where otb applications are installed
  set(PKG_OTBLIBS_DIR "${PKG_STAGE_DIR}/lib/otb")

  #<prefix>/share for gdal data files
  set(PKG_SHARE_DEST_DIR ${PKG_STAGE_DIR}/share)

  set(PKG_SHARE_SOURCE_DIR ${DEPENDENCIES_INSTALL_DIR}/share)

  if(NOT OTB_APPS_LIST)
    message(FATAL_ERROR "you must set 'OTB_APPS_LIST' before calling this method")
  endif()

  # Just check if required variables are defined.
  foreach(req
      DEPENDENCIES_INSTALL_DIR
      OTB_APPLICATIONS_DIR
      PKG_STAGE_DIR
      PACKAGE_SUPPORT_FILES_DIR
      CMAKE_INSTALL_PREFIX
      OTB_INSTALL_DIR
      )
    if(NOT DEFINED ${req})
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  # one for debugging..
  # install(CODE "message(\"CMake/PackageHelper.cmake:install_common(${outdir})\n${vars}\n\")")

  ##################### install environment source ##########################
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(ENV_SOURCE_FILES
      "${PACKAGE_SUPPORT_FILES_DIR}/otbenv.cmd"
      "${PACKAGE_SUPPORT_FILES_DIR}/otbenv.profile"
      )

  elseif(UNIX)
    set(ENV_SOURCE_FILES "${PACKAGE_SUPPORT_FILES_DIR}/otbenv.profile")
  endif()

  foreach(ENV_SOURCE_FILE ${ENV_SOURCE_FILES})
    if(EXISTS ${ENV_SOURCE_FILE})
      install(FILES ${ENV_SOURCE_FILE} DESTINATION ${PKG_STAGE_DIR})
    endif()
  endforeach()

  ####################### install cli and gui scripts ###########################
  file(GLOB PKG_APP_SCRIPTS
    ${OTB_INSTALL_DIR}/bin/otbcli*
    ${OTB_INSTALL_DIR}/bin/otbgui*) #

  list(LENGTH PKG_APP_SCRIPTS PKG_APP_SCRIPTS_LENGTH)
  if (PKG_APP_SCRIPTS_LENGTH LESS 1)
    message(WARNING "PKG_APP_SCRIPTS is empty: ${PKG_APP_SCRIPTS}")
  endif()

  ##################### install cli and gui scripts #######################
  install(FILES ${PKG_APP_SCRIPTS}  DESTINATION ${PKG_STAGE_BIN_DIR})

  if(include_mvd)
    install_monteverdi_files()
  endif()

  ####################### install GDAL data ############################
  set(GDAL_DATA ${PKG_SHARE_SOURCE_DIR}/gdal)
  if(NOT EXISTS "${GDAL_DATA}/epsg.wkt")
    message(FATAL_ERROR "Cannot generate package without GDAL_DATA : ${GDAL_DATA} ${DEPENDENCIES_INSTALL_DIR}")
  endif()

  install(DIRECTORY ${GDAL_DATA} DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install GeoTIFF data ###########################
  install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/epsg_csv DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install OSSIM data ###########################
  install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/ossim DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### Install otb applications ######################
  install(DIRECTORY "${OTB_APPLICATIONS_DIR}"  DESTINATION ${PKG_OTBLIBS_DIR})

endfunction()

function(install_monteverdi_files)

  #name/ext of qt's sqlite plugin. Varies with platform/OS
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(PKG_QTSQLITE_FILENAME "qsqlite4.dll")
  elseif(APPLE)
    set(PKG_QTSQLITE_FILENAME "qsqlite.dylib")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND NOT CMAKE_CROSSCOMPILING)
    set(PKG_QTSQLITE_FILENAME "libqsqlite.so")
  else()
    message(FATAL_ERROR "install_monteverdi_files: Unknown OS/Platform")
  endif()

  #root folder where qt plugins are installed
  set(PKG_QTPLUGINS_DIR "${PKG_STAGE_DIR}/lib/qt4/plugins")

  #qt4's distributes some translation of common message string used in Qt.
  #This are provided with any qt installation. We reuse them in otb package
  #so as not to reinvent the wheels.
  set(PKG_I18N_DIR "${PKG_STAGE_DIR}/lib/qt4/translations")

  #<prefix>/share for otb i18n directory. This is different from qt's i18N directory
  #which is <prefix>/share/qt4/translations.
  set(PKG_OTB_I18N_DIR "${PKG_STAGE_DIR}/${Monteverdi_INSTALL_DATA_DIR}/i18n")

  # Just check if required variables are defined.
  foreach(req
      Monteverdi_SOURCE_DIR
      QT_PLUGINS_DIR
      PKG_STAGE_BIN_DIR
      PKG_QTSQLITE_FILENAME
      PKG_QTPLUGINS_DIR
      PKG_I18N_DIR
      PKG_OTB_I18N_DIR
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  ####################### install mingw qt.conf ##########################
  if(EXISTS ${Monteverdi_SOURCE_DIR}/Packaging/Windows/mingw/qt.conf)
    install(FILES ${Monteverdi_SOURCE_DIR}/Packaging/Windows/mingw/qt.conf
      DESTINATION ${PKG_STAGE_BIN_DIR})
  endif()

  ####################### install sqldriver plugin ########################
  install(FILES ${QT_PLUGINS_DIR}/sqldrivers/${PKG_QTSQLITE_FILENAME}
    DESTINATION ${PKG_QTPLUGINS_DIR}/sqldrivers)

  ####################### install translations ###########################
  #get all translations already distributed with qt4
  get_qt_translation_files(QT_TRANSLATIONS_FILES)

  #install all files in ${QT_TRANSLATIONS_FILES}
  install(FILES ${QT_TRANSLATIONS_FILES}  DESTINATION ${PKG_I18N_DIR})

  #translation of monteverdi specific strings
  file(GLOB APP_TS_FILES ${Monteverdi_SOURCE_DIR}/i18n/*.ts) # qm files
  foreach(APP_TS_FILE ${APP_TS_FILES})
    get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
    install(FILES ${Monteverdi_BINARY_DIR}/i18n/${APP_TS_FILENAME}.qm
      DESTINATION ${PKG_OTB_I18N_DIR})
  endforeach()

endfunction()

macro(empty_package_staging_directory)
  message(STATUS "Empty package staging directory: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
endmacro()

function(configure_package)
  set(EXE_EXT "")
  set(LIB_EXT "*so")
  set(SCR_EXT ".sh")
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(EXE_EXT ".exe")
    set(LIB_EXT "*dll")
    set(SCR_EXT ".bat")
  endif()

  #This must exist in any OTB Installation minimal or full
  set(PKG_BINARY_FILES "bin/otbApplicationLauncherCommandLine")
  set(PKG_PEFILES "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT}")
  if(NOT EXISTS "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT}")
    message(FATAL_ERROR "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT} not found.")
  endif()

  foreach(EXE_FILE otbApplicationLauncherQt
      iceViewer
      otbTestDriver)
    if(EXISTS "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
      #see the first comment about PKG_BINARY_FILES
      set(PKG_BINARY_FILES "${PKG_BINARY_FILES} bin/${EXE_FILE}${EXE_EXT}")
      list(APPEND PKG_PEFILES
        "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
    endif()
  endforeach()

  foreach(EXE_FILE monteverdi
      mapla)
    if(EXISTS "${CMAKE_INSTALL_PREFIX}/bin/${EXE_FILE}${EXE_EXT}")
      #PKG_BINARY_FILES might seem a bit redundant variable if you
      #consider PKG_PEFILES which also has same content.
      #But PKG_BINARY_FILES goes into pkgsetup.in for Linux standalone binaries
      # and other one (PKG_PEFILES) is for dependency resolution in
      # process_deps() function
      set(PKG_BINARY_FILES "${PKG_BINARY_FILES} bin/${EXE_FILE}${EXE_EXT}")
      list(APPEND PKG_PEFILES
        "${CMAKE_INSTALL_PREFIX}/bin/${EXE_FILE}${EXE_EXT}")
    endif()
    if(DEFINED Monteverdi_SOURCE_DIR)
      if(EXISTS ${Monteverdi_SOURCE_DIR}/Packaging/${EXE_FILE}${SCR_EXT})
        install(PROGRAMS
          ${Monteverdi_SOURCE_DIR}/Packaging/${EXE_FILE}${SCR_EXT}
          DESTINATION
          "${PKG_STAGE_DIR}")
      endif()
    endif()
  endforeach()

  file(GLOB OTB_APPS_LIST ${OTB_APPLICATIONS_DIR}/otbapp_${LIB_EXT}) # /lib/otb

  #see the first comment about PKG_BINARY_FILES
  foreach(OTB_APP_SO ${OTB_APPS_LIST})
    get_filename_component(OTB_APP_SO_NAME ${OTB_APP_SO} NAME)
    set(PKG_BINARY_FILES "${PKG_BINARY_FILES} lib/otb/applications/${OTB_APP_SO_NAME}")
  endforeach()

  set(include_mvd 0)
  if(DEFINED Monteverdi_SOURCE_DIR)
    set(include_mvd 1)
  endif()

  list(APPEND PKG_PEFILES ${OTB_APPS_LIST})

  install_common(${include_mvd})

  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/temp_so_names_dir")
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/temp_so_names_dir")

  set(alldlls)
  set(notfound_dlls)
  foreach(infile ${PKG_PEFILES})
    get_filename_component(bn ${infile} NAME)
    process_deps(${bn})
  endforeach()

  list(LENGTH notfound_dlls nos)
  if(${nos} GREATER 0)
    STRING(REPLACE ".so;" ".so," notfound ${notfound_dlls})
    message(FATAL_ERROR "Following dlls were not found: ${notfound_dlls}. Please consider adding their paths to SEARCHDIRS when calling superbuild_package macro.")
  endif()

  file(GLOB temp_files "${CMAKE_BINARY_DIR}/temp_so_names_dir/*") # /lib/otb
  foreach(temp_file ${temp_files})
    get_filename_component(basename_of_temp_file ${temp_file} NAME)
    set(PKG_BINARY_FILES "${PKG_BINARY_FILES} lib/${basename_of_temp_file}")
  endforeach()

  #remove this temporary directory
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/temp_so_names_dir")

  set(PKG_BINARY_FILES "${PKG_BINARY_FILES}" PARENT_SCOPE)

endfunction()

# The below function is modified from GetPrerequisities.cmake
# which is distributed with CMake.
function(is_file_a_symbolic_link file result_var1 result_var2)
  #
  # A file is not executable until proven otherwise:
  #
  set(${result_var1} 0 PARENT_SCOPE)
  set(${result_var2} "" PARENT_SCOPE)

  get_filename_component(file_full "${file}" ABSOLUTE)
  string(TOLOWER "${file_full}" file_full_lower)

  # If file name ends in .exe on Windows, *assume* executable:
  #
  if(WIN32 AND NOT UNIX)
    if("${file_full_lower}" MATCHES "\\.lnk$")
      set(${result_var1} 1 PARENT_SCOPE)
      #Assuming the file is linked to a file with same name without .lnk extension
      get_filename_component(name_we_lnk "${file_full_lower}" NAME_WE)
      set(${result_var2} "${name_we_lnk}" PARENT_SCOPE)
      return()
    endif()

    # A clause could be added here that uses output or return value of dumpbin
    # to determine ${result_var}. In 99%+? practical cases, the exe name
    # match will be sufficient...
    #
  endif()

  # Use the information returned from the Unix shell command "file" to
  # determine if ${file_full} should be considered an executable file...
  #
  # If the file command's output contains "executable" and does *not* contain
  # "text" then it is likely an executable suitable for prerequisite analysis
  # via the get_prerequisites macro.
  #
  if(UNIX)
    if(NOT file_cmd)
      find_program(file_cmd "file")
      mark_as_advanced(file_cmd)
    endif()

    if(file_cmd)
      execute_process(COMMAND "${file_cmd}" "${file_full}"
        RESULT_VARIABLE file_rv
        OUTPUT_VARIABLE file_ov
        ERROR_VARIABLE file_ev
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
      if(NOT file_rv STREQUAL "0")
        message(FATAL_ERROR "${file_cmd} failed: ${file_rv}\n${file_ev}")
      endif()

      # Replace the name of the file in the output with a placeholder token
      # (the string " _file_full_ ") so that just in case the path name of
      # the file contains the word "text" or "executable" we are not fooled
      # into thinking "the wrong thing" because the file name matches the
      # other 'file' command output we are looking for...
      #
      string(REPLACE "${file_full}" " _file_full_ " file_ov "${file_ov}")
      string(TOLOWER "${file_ov}" file_ov_lower)

      #message(FATAL_ERROR "file_ov='${file_ov}'")
      if("${file_ov_lower}" MATCHES "symbolic link")
        #message(STATUS "symbolic link!")
        set(${result_var1} 1 PARENT_SCOPE)
        #Now find where the symlink is linked to.
        #Do a regex replace
        string(REGEX REPLACE "_file_full_*.*symbolic.link.to.."
          "" symlinked_to ${file_ov})
        #Take out last character which is a single quote
        string(REPLACE "'" "" symlinked_to "${symlinked_to}")
        #strip for our own sanity
        string(STRIP ${symlinked_to} symlinked_to)
        set(${result_var2} "${symlinked_to}" PARENT_SCOPE)
        #message(FATAL_ERROR "${file_full} is symlinked_to ${symlinked_to}")
        return()
      endif()

    else()
      message(STATUS "warning: No 'file' command, skipping execute_process...")
    endif()
  endif()
endfunction()
