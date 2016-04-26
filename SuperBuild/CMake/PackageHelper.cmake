set(WINDOWS_SYSTEM_DLLS
  msvc.*dll
  user32.dll
  gdi32.dll
  shell32.dll
  kernel32.dll
  ws2_32.dll
  wldap32.dll
  ole32.dll
  comdlg32.dll
  shfolder.dll
  secur32.dll
  wsock32.dll
  advapi32.dll
  crypt32.dll
  imm32.dll
  oleaut32.dll
  winmm.dll
  opengl32.dll
  glu32.dll
  winspool.drv)

set(LINUX_SYSTEM_DLLS
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

# libgcc_s.*dylib and other *.framework are dragged by QT
set(APPLE_SYSTEM_DLLS
  libSystem.*dylib
  libiconv.*dylib
  libc\\+\\+.*dylib
  libstdc.*dylib
  libobjc.*dylib
  ApplicationServices.framework
  CoreFoundation.framework
  CoreServices.framework
  Security.framework
  Carbon.framework
  AppKit.framework
  Foundation.framework
  AGL.framework
  OpenGL.framework
  libgcc_s.*dylib
  )

if(WIN32 OR CMAKE_CROSSCOMPILING)
  set(SYSTEM_DLLS "${WINDOWS_SYSTEM_DLLS}")
else() #case for unixes
  if(APPLE)
    set(SYSTEM_DLLS "${APPLE_SYSTEM_DLLS}")
  else()
    set(SYSTEM_DLLS "${LINUX_SYSTEM_DLLS}")
  endif()
endif(WIN32 OR CMAKE_CROSSCOMPILING)

macro(is_system_dll matched value)
  set(${matched})
  string(TOLOWER ${value} value_)
  foreach (pattern ${SYSTEM_DLLS})
    string(TOLOWER ${pattern} pattern_)
    if("${value_}" MATCHES "${pattern_}")
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
      OTB_INSTALL_DIR
      )
    if(NOT DEFINED ${req})
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  # one for debugging..
  # install(CODE "message(\"CMake/PackageHelper.cmake:install_common(${outdir})\n${vars}\n\")")

  set(PKG_OTB_SHARE_SOURCE_DIR "${PKG_SHARE_SOURCE_DIR}")

  #For Unixes we make them in the *pkgsetup.in
  ##################### install environment source ##########################
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    foreach(ENV_SOURCE_FILE
        "${PACKAGE_SUPPORT_FILES_DIR}/otbenv.cmd"
        "${PACKAGE_SUPPORT_FILES_DIR}/otbenv.profile")
      if(EXISTS ${ENV_SOURCE_FILE})
        install(FILES ${ENV_SOURCE_FILE} DESTINATION ${PKG_STAGE_DIR})
      endif()
    endforeach()
    set(PKG_OTB_SHARE_SOURCE_DIR "${CMAKE_INSTALL_PREFIX}/share")
  endif()
  ####################### install cli and gui scripts ###########################
  file(GLOB PKG_APP_SCRIPTS
    ${OTB_INSTALL_DIR}/bin/otbcli*
    ${OTB_INSTALL_DIR}/bin/otbgui*) #

  list(LENGTH PKG_APP_SCRIPTS PKG_APP_SCRIPTS_LENGTH)
  if (PKG_APP_SCRIPTS_LENGTH LESS 1)
    message(WARNING "PKG_APP_SCRIPTS is empty: ${PKG_APP_SCRIPTS}")
  endif()

  ##################### install cli and gui scripts #######################
  install(PROGRAMS ${PKG_APP_SCRIPTS}  DESTINATION ${PKG_STAGE_BIN_DIR})

  if(include_mvd)
    install_monteverdi_files()
    if(NOT EXISTS "${PKG_OTB_SHARE_SOURCE_DIR}/otb/i18n")
      message(FATAL_ERROR "error ${PKG_OTB_SHARE_SOURCE_DIR}/otb not exists")
    endif()
  endif()

  ####################### install GDAL data ############################
  set(GDAL_DATA ${PKG_SHARE_SOURCE_DIR}/gdal)
  if(NOT EXISTS "${GDAL_DATA}/epsg.wkt")
    message(FATAL_ERROR "Cannot generate package without GDAL_DATA : ${GDAL_DATA} ${DEPENDENCIES_INSTALL_DIR}")
  endif()

  install(DIRECTORY ${GDAL_DATA} DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install GeoTIFF data ########################
  install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/epsg_csv DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install OSSIM data ##########################
  install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/ossim DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install otb share ###########################
  install(DIRECTORY ${PKG_OTB_SHARE_SOURCE_DIR}/otb DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install proj share ##########################
  if(EXISTS ${PKG_SHARE_SOURCE_DIR}/proj)
    install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/proj DESTINATION ${PKG_SHARE_DEST_DIR})
  endif()

  ####################### Install otb applications ####################
  install(DIRECTORY "${OTB_APPLICATIONS_DIR}"  DESTINATION ${PKG_OTBLIBS_DIR})

  ####################### Install copyrights ##########################
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    #do install license for windows package
  else()
    install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/copyright DESTINATION ${PKG_SHARE_DEST_DIR})
    install(FILES ${PKG_SHARE_SOURCE_DIR}/copyright/LICENSE DESTINATION ${PKG_STAGE_DIR})
  endif()

endfunction()

function(install_monteverdi_files)

  #name/ext of qt's sqlite plugin. Varies with platform/OS
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(PKG_QTSQLITE_FILENAME "qsqlite4.dll")
  elseif(APPLE)
    set(PKG_QTSQLITE_FILENAME "libqsqlite.dylib")
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

  #install icon file for .app file. Monteverdi and Mapla has same icon!
  if(APPLE)
    install(FILES ${Monteverdi_SOURCE_DIR}/Packaging/MacOS/Monteverdi.icns
      DESTINATION ${PKG_STAGE_DIR})
  endif()

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

#NOTE:
# VAR_IN_PKGSETUP_CONFIGURE cmake variable is set below.
# This is important and useful running configure_file()
# over *pkgsetup.in

function(configure_package)
  if(UNIX)
    set(EXE_EXT "")
    set(SCR_EXT ".sh")
    if(APPLE)
      set(LIB_EXT "*dylib")
    else()
      set(LIB_EXT "*so")
    endif()
  endif()

  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(EXE_EXT ".exe")
    set(LIB_EXT "*dll")
    set(SCR_EXT ".bat")
  endif()

  #This must exist in any OTB Installation minimal or full
  set(VAR_IN_PKGSETUP_CONFIGURE "bin/otbApplicationLauncherCommandLine")
  set(PKG_PEFILES "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT}")
  if(NOT EXISTS "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT}")
    message(FATAL_ERROR "${OTB_INSTALL_DIR}/bin/otbApplicationLauncherCommandLine${EXE_EXT} not found.")
  endif()

  foreach(EXE_FILE otbApplicationLauncherQt
      iceViewer
      otbTestDriver)
    if(EXISTS "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
      #see the first comment about VAR_IN_PKGSETUP_CONFIGURE
      set(VAR_IN_PKGSETUP_CONFIGURE "${VAR_IN_PKGSETUP_CONFIGURE} bin/${EXE_FILE}${EXE_EXT}")
      list(APPEND PKG_PEFILES
        "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
    endif()
  endforeach()

  foreach(EXE_FILE monteverdi
      mapla)
    if(EXISTS "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
      #VAR_IN_PKGSETUP_CONFIGURE might seem a bit redundant variable if you
      #consider PKG_PEFILES which also has same content.
      #But VAR_IN_PKGSETUP_CONFIGURE goes into pkgsetup.in for Linux standalone binaries
      # and other one (PKG_PEFILES) is for dependency resolution in
      # process_deps() function
      set(VAR_IN_PKGSETUP_CONFIGURE "${VAR_IN_PKGSETUP_CONFIGURE} bin/${EXE_FILE}${EXE_EXT}")
      list(APPEND PKG_PEFILES
        "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
    endif()
    #For Unixes we write the startup script in the *pkgsetup.in
    if(WIN32 OR CMAKE_CROSSCOMPILING)
      if(DEFINED Monteverdi_SOURCE_DIR)
        if(EXISTS ${Monteverdi_SOURCE_DIR}/Packaging/${EXE_FILE}${SCR_EXT})
          install(PROGRAMS
            ${Monteverdi_SOURCE_DIR}/Packaging/${EXE_FILE}${SCR_EXT}
            DESTINATION
            "${PKG_STAGE_DIR}")
        endif()
      endif()
    endif(WIN32 OR CMAKE_CROSSCOMPILING)
  endforeach()

  file(GLOB OTB_APPS_LIST ${OTB_APPLICATIONS_DIR}/otbapp_${LIB_EXT}) # /lib/otb

  #see the first comment about VAR_IN_PKGSETUP_CONFIGURE
  foreach(OTB_APP_SO ${OTB_APPS_LIST})
    get_filename_component(OTB_APP_SO_NAME ${OTB_APP_SO} NAME)
    set(VAR_IN_PKGSETUP_CONFIGURE "${VAR_IN_PKGSETUP_CONFIGURE} lib/otb/applications/${OTB_APP_SO_NAME}")
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
    list(REMOVE_DUPLICATES notfound_dlls)
    #string(REPLACE ";" "\r" notfound_dlls ${notfound_dlls})
    message(FATAL_ERROR "Following dlls were not found: ${notfound_dlls}. Please consider adding their paths to PKG_SEARCHDIRS when calling superbuild_package macro.")
  endif()

  file(GLOB temp_files "${CMAKE_BINARY_DIR}/temp_so_names_dir/*") # /lib/otb
  foreach(temp_file ${temp_files})
    get_filename_component(basename_of_temp_file ${temp_file} NAME)
    set(VAR_IN_PKGSETUP_CONFIGURE "${VAR_IN_PKGSETUP_CONFIGURE} lib/${basename_of_temp_file}")
  endforeach()

  #remove this temporary directory
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/temp_so_names_dir")

  set(VAR_IN_PKGSETUP_CONFIGURE "${VAR_IN_PKGSETUP_CONFIGURE}" PARENT_SCOPE)

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
        if(UNIX)
          if(APPLE)
            string(REGEX REPLACE "_file_full_*.*symbolic.link.to."
              "" symlinked_to ${file_ov})
          else(APPLE)
            string(REGEX REPLACE "_file_full_*.*symbolic.link.to.."
              "" symlinked_to ${file_ov})
          endif(APPLE)
        endif(UNIX)
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



function(process_deps infile)

  if(APPLE)
    if( "${infile}" MATCHES "@rpath")
      string(REGEX REPLACE "@rpath." "" infile "${infile}")
    endif()
  endif()
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    string(TOLOWER "${infile}" infile_lower )
  endif()
  get_filename_component(bn ${infile} NAME)

  list_contains(contains "${bn}" "${alldlls}")
  if(NOT contains)
    set(DLL_FOUND FALSE)

    foreach(SEARCHDIR ${PKG_SEARCHDIRS})
      if(NOT DLL_FOUND)
        if(WIN32 OR CMAKE_CROSSCOMPILING)
          if(NOT EXISTS ${SEARCHDIR}/${infile} )
            if(EXISTS ${SEARCHDIR}/${infile_lower} )
              set(infile ${infile_lower})
            endif()
          endif()
        endif()
        if(EXISTS ${SEARCHDIR}/${infile})
          set(DLL_FOUND TRUE)
          message(STATUS "Processing ${SEARCHDIR}/${infile}")
          is_file_executable("${SEARCHDIR}/${infile}" is_executable)
          if(is_executable)
              install(PROGRAMS "${SEARCHDIR}/${infile}"
                DESTINATION ${PKG_STAGE_DIR}/bin)
          else(is_executable)
            get_filename_component(bn_we ${infile} NAME_WE)
            file(GLOB sofiles "${SEARCHDIR}/${bn_we}*")
            foreach(sofile ${sofiles})
              get_filename_component(sofile_ext ${sofile} EXT)
              set(is_valid TRUE)
              if ("${sofile_ext}" MATCHES ".la"
                  OR "${sofile_ext}" MATCHES ".prl"
                  OR "${sofile_ext}" MATCHES ".a")
                set(is_valid FALSE)
              endif()

              if(is_valid)
                get_filename_component(basename_of_sofile ${sofile} NAME)
                is_file_a_symbolic_link("${sofile}" is_symlink linked_to_file)
                if(is_symlink)
                  # NOTE: $OUT_DIR is set actually in pkgsetup.in. So don't try
                  # any pre-mature optimization on that variable names
                  file(APPEND
                    ${CMAKE_BINARY_DIR}/make_symlinks
                    "ln -sf $OUT_DIR/lib/${linked_to_file} $OUT_DIR/lib/${basename_of_sofile}\n"
                    )
                  #message("${sofile} is a symlink to ${linked_to_file}")
                else() # is_symlink
                  if("${basename_of_sofile}" MATCHES "otbapp_")
                    if(NOT PKG_GENERATE_XDK)
                      install(FILES "${sofile}" DESTINATION ${PKG_STAGE_DIR}/lib/otb/applications)
                    endif()
                  else() #if(.. MATCHES "otbapp_")
                    #if we are making xdk. skill all those starting with libotb case insensitively
                    if(PKG_GENERATE_XDK)
                      string(TOLOWER "${basename_of_sofile}" sofile_lower )
                      if(NOT "${sofile_lower}" MATCHES "libotb")
                        install(FILES "${sofile}" DESTINATION ${PKG_STAGE_DIR}/lib)
                      endif()
                    else() #PKG_GENERATE_XDK
                      #just install the so file to <staging-dir>/lib
                      install(FILES "${sofile}" DESTINATION ${PKG_STAGE_DIR}/lib)
                    endif() #PKG_GENERATE_XDK

                    # Finally touch a file in temp directory for globbing later
                    # message("touching ${basename_of_sofile}")
                    execute_process(COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_BINARY_DIR}/temp_so_names_dir/${basename_of_sofile}")
                  endif() #if(.. MATCHES "otbapp_")
                endif() #is_symlink
              endif() #is_valid
            endforeach()
          endif(is_executable)
          execute_process(
            COMMAND ${LOADER_PROGRAM} ${LOADER_PROGRAM_ARGS} "${SEARCHDIR}/${infile}"
            RESULT_VARIABLE loader_rv
            OUTPUT_VARIABLE loader_ov
            ERROR_VARIABLE loader_ev
            )
          if(loader_rv)
            message(FATAL_ERROR "loader_ev=${loader_ev}\n PACKAGE-OTB: result_variable is '${loader_rv}'")
          endif()

          if(WIN32 OR CMAKE_CROSSCOMPILING)
            string(REGEX MATCHALL "DLL.Name..[A-Za-z(0-9\\.0-9)+_\\-]*" loader_ov "${loader_ov}")
            string(REGEX REPLACE "DLL.Name.." "" needed_dlls "${loader_ov}")
          else()  #case for unixes
            if(APPLE)
              string(REGEX REPLACE "[^\n]+cmd LC_LOAD_DYLIB\n[^\n]+\n[^\n]+name ([^\n]+).\\(offset[^\n]+\n" "rpath \\1\n" loader_ov "${loader_ov}")
              string(REGEX MATCHALL "rpath [^\n]+" loader_ov "${loader_ov}")
              string(REGEX REPLACE "rpath " "" needed_dlls "${loader_ov}")
            else(APPLE)
              string(REGEX MATCHALL "NEEDED\\ *[A-Za-z(0-9\\.0-9)+_\\-]*" loader_ov "${loader_ov}")
              string(REGEX REPLACE "NEEDED" "" needed_dlls "${loader_ov}")
            endif(APPLE)
          endif()

          foreach(needed_dll ${needed_dlls})
            string(STRIP ${needed_dll} needed_dll)
            process_deps(${needed_dll})
          endforeach()
        endif()
      endif(NOT DLL_FOUND)
    endforeach()

    if(NOT DLL_FOUND)
      is_system_dll(iss "${infile}")
      if(NOT iss)
        set(notfound_dlls "${notfound_dlls};${infile}")
      endif()
    else(NOT DLL_FOUND)
      set( alldlls "${alldlls};${bn}" PARENT_SCOPE )
    endif(NOT DLL_FOUND)
    set(notfound_dlls "${notfound_dlls}" PARENT_SCOPE )
  endif()
endfunction()