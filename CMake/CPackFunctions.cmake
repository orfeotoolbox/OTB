

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


function(create_cpack_config application)

  #should we handle this when calling function ?
  #for now mapla has no specific version or it is same as monteverdi
  SET(CPACK_PACKAGE_VERSION "${Monteverdi_VERSION_MAJOR}.${Monteverdi_VERSION_MINOR}.${Monteverdi_VERSION_PATCH}")
  SET(CPACK_PACKAGE_VERSION_MAJOR "${Monteverdi_VERSION_MAJOR}")
  SET(CPACK_PACKAGE_VERSION_MINOR "${Monteverdi_VERSION_MINOR}")
  SET(CPACK_PACKAGE_VERSION_PATCH "${Monteverdi_VERSION_PATCH}")

  if(WIN32)
    set(arch_prefix win32)
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
      set(arch_prefix win64)
    endif()
    SET(CPACK_GENERATOR "NSIS")
    STRING(TOLOWER ${application} application_)
    SET(BATFILE_NAME "${application_}.bat")
    SET(EXEFILE_NAME "${application_}.exe")
    SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
    "Delete \\\"$SMPROGRAMS\\\\${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}\\\\${application}.lnk\\\"  ")
    SET(CPACK_NSIS_MENU_LINKS "bin/${BATFILE_NAME}" "${application}" )
    SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\${EXEFILE_NAME}")
    SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
      "CreateShortCut \\\"$SMPROGRAMS\\\\${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}\\\\${application}.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${BATFILE_NAME}\\\" \\\" \\\" \\\"$INSTDIR\\\\bin\\\\${EXEFILE_NAME}\\\"
    ")

  else(APPLE)
      set(arch_prefix Darwin)
     if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(arch_prefix Darwin64)
    endif()
    SET(CPACK_GENERATOR "Bundle")
    SET(CPACK_BUNDLE_ICON "${Monteverdi_SOURCE_DIR}/Packaging/MacOS/Monteverdi.icns" )
    SET(CPACK_BUNDLE_NAME "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${Monteverdi_VERSION_MINOR}" )
    SET(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Packaging/MacOS/${application}-Info.plist" )
    SET(CPACK_BUNDLE_STARTUP_COMMAND "${Monteverdi_SOURCE_DIR}/Packaging/MacOS/${application}-StartupCommand" )

  endif()

  #common cpack configurations.

  ##################################
  ##################################
  #CAUTION: Be careful when changing values below.#
  ##################################
  ##################################

  SET(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
  SET(CPACK_MODULE_PATH "${Monteverdi_SOURCE_DIR}/CMake")
  SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
  SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
  SET(CPACK_PACKAGE_CONTACT "contact@orfeo-toolbox.org")
  SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${Monteverdi_SOURCE_DIR}/Description.txt")

  SET(CPACK_RESOURCE_FILE_LICENSE "${Monteverdi_SOURCE_DIR}/Copyright/Copyright.txt")
  SET(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_BINARY_DIR}/CPackConfig-${application}.cmake")
  SET(CPACK_NSIS_MUI_ICON    "${Monteverdi_SOURCE_DIR}/Data/Icons/monteverdi.ico")
  SET(CPACK_NSIS_MUI_UNIICON "${Monteverdi_SOURCE_DIR}/Data/Icons/monteverdi.ico")

  SET(CPACK_COMPONENTS_ALL)
  LIST(APPEND CPACK_COMPONENTS_ALL  "Resources")
  LIST(APPEND CPACK_COMPONENTS_ALL "Runtime")
  LIST(APPEND CPACK_COMPONENTS_ALL "${application}")

  SET(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Runtime)
  SET(CPACK_COMPONENT_RUNTIME_DEPENDS Resources)

  SET(CPACK_COMPONENT_RUNTIME_REQUIRED ON)
  SET(CPACK_COMPONENT_RESOURCES_REQUIRED ON)
  SET(CPACK_COMPONENT_RESOURCES_HIDDEN ON)

  SET(CPACK_COMPONENT_MAPLA_REQUIRED ON)
  SET(CPACK_COMPONENT_MONTEVERDI_REQUIRED ON)

  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${application};ALL;/")

  SET(CPACK_PACKAGE_NAME "${application}")

  SET(CPACK_NSIS_DISPLAY_NAME "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_NSIS_PACKAGE_NAME "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")

  SET(CPACK_PACKAGE_FILE_NAME "${application}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${arch_prefix}")

  INCLUDE(InstallRequiredSystemLibraries)
  INCLUDE(CPack)

endfunction(create_cpack_config)


function(configure_app_package app need_apps)


  string(TOLOWER ${app} EXECUTABLE_NAME)

  if(WIN32)
    file(TO_CMAKE_PATH "$ENV{OSGEO4W_ROOT}" OSGEO4W_ROOT)
    if(NOT OSGEO4W_ROOT)
      message(FATAL_ERROR "Cannot generate installer without OSGeo4W environment : OSGEO4W_ROOT")
    endif(NOT OSGEO4W_ROOT)

    # The following DLL handling is very specific
    # both to OSGeo4W flavour (32 bit or 64 bit), OSGeo4W package state
    # and the compiler used to make the Monteverdi installer (VC2010)
    # Don't expect too much of it without VC2010.

    # For some reason, fixup_bundle won't package the msvc runtime dll
    # I guess it's because cpack already handles installing the runtime,
    # but here we build with a particular version of Visual, but
    # some OSGeo4W dependencies are built upon other runtimes
    set(MVD2_ADDITIONNAL_DLLS
      ${OSGEO4W_ROOT}/bin/msvcp60.dll
      ${OSGEO4W_ROOT}/bin/msvcp70.dll
      ${OSGEO4W_ROOT}/bin/msvcp71.dll
      ${OSGEO4W_ROOT}/bin/msvcr71.dll
      ${OSGEO4W_ROOT}/bin/msvcrt.dll)

    # Since an upgrade in OSGeo4W-32bit of libpq (around 2013/10)
    # libpq depends on two additional libs that cmake
    # surprisingly miss during package generation
    # I really don't understand why.
    # Let's add them manually

    set(MVD2_ADDITIONNAL_DLLS
      ${MVD2_ADDITIONNAL_DLLS}
      ${OSGEO4W_ROOT}/bin/libgcc_s_dw2-1.dll
      ${OSGEO4W_ROOT}/bin/libiconv-2.dll)

    foreach(dll ${MVD2_ADDITIONNAL_DLLS})
      if (EXISTS ${dll})
        install( FILES ${dll} DESTINATION bin COMPONENT Runtime )
      endif()
    endforeach()

    ## set(APP_PREFIX_DIR lib)

    set(APP_QTCONF_DIR bin)
    set(APP_QTSQLITE_FILENAME qsqlite4.dll)
    set(APP_QTPLUGINS_DIR lib/qt4/plugins)
    set(APP_OTBLIBS_DIR lib/otb)
    set(APP_I18N_DIR lib/qt4/translations)
    set(APP_DATA_DIR share)
    set(APP_NAME "\${CMAKE_INSTALL_PREFIX}/bin/${EXECUTABLE_NAME}.exe")

    install(
      CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${APP_QTCONF_DIR}/qt.conf\"
                \"[Paths]
                  Translations=../lib/qt4/translations
                  Plugins=../lib/qt4/plugins
                \")"
      COMPONENT Runtime)

    install(
      FILES ${Monteverdi_SOURCE_DIR}/Packaging/Windows/${EXECUTABLE_NAME}.bat
      DESTINATION bin
      COMPONENT ${app})

    if(Monteverdi_USE_CPACK )
      set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${app}")
      create_cpack_config(${app})
    endif()

  endif(WIN32)

  if(APPLE)
    set(APP_NAME "\${CMAKE_INSTALL_PREFIX}/${app}.app")
    set(APP_PREFIX_DIR ${app}.app/Contents) #osx prefix
    set(APP_QTCONF_DIR ${APP_PREFIX_DIR}/Resources)
    set(APP_QTSQLITE_FILENAME libqsqlite.dylib)
    set(APP_QTPLUGINS_DIR ${APP_PREFIX_DIR}/plugins)
    set(APP_OTBLIBS_DIR ${APP_PREFIX_DIR}/MacOS/otb)
    set(APP_I18N_DIR ${APP_PREFIX_DIR}/Resources/otb/translations) #translations
    set(APP_DATA_DIR ${APP_PREFIX_DIR}/Resources/gdal)

    install(
      FILES ${CMAKE_SOURCE_DIR}/Packaging/MacOS/Monteverdi.icns
      DESTINATION ${APP_PREFIX_DIR}/Resources
      COMPONENT Resources)

    install(
      FILES ${CMAKE_BINARY_DIR}/Code/Application/${app}/${EXECUTABLE_NAME}_start
      DESTINATION ${APP_PREFIX_DIR}/MacOS
      PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      COMPONENT Runtime)

    install(
      CODE
      "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${APP_QTCONF_DIR}/qt.conf\"
     \"[Paths]
       Translations=Resources/otb/translations
       Plugins=plugins
     \")"
      COMPONENT Runtime)

    file(GLOB APP_QM_TRANSLATIONS /*.qm) # qm files

    file(GLOB APP_TS_FILES ${CMAKE_SOURCE_DIR}/i18n/*.ts) # qm files

    foreach(APP_TS_FILE ${APP_TS_FILES})
      get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
      install(
        FILES ${CMAKE_BINARY_DIR}/i18n/${APP_TS_FILENAME}.qm
        DESTINATION ${APP_PREFIX_DIR}/${Monteverdi_INSTALL_DATA_DIR}/i18n/
        COMPONENT Resources)
    endforeach()

    set(CPACK_BINARY_DRAGNDROP ON)

  endif(APPLE)

  ################################################################################
  ################################################################################
  ################### END OF PLATFORM SPECIFIC CMAKE VARIABLES ###################
  ################################################################################
  ################################################################################

  ####################### install sqldrivers plugin #######################

  install(
    FILES ${QT_PLUGINS_DIR}/sqldrivers/${APP_QTSQLITE_FILENAME}
    DESTINATION ${APP_QTPLUGINS_DIR}/sqldrivers
    COMPONENT Runtime)

  ####################### install translations #######################
  get_qt_translation_files(QT_TRANSLATIONS_FILES)

  install(
    FILES ${QT_TRANSLATIONS_FILES}
    DESTINATION ${APP_I18N_DIR}
    COMPONENT Resources)

  ####################### install GDAL data #######################

  file(TO_CMAKE_PATH "$ENV{GDAL_DATA}" GDAL_DATA)
  if(NOT GDAL_DATA)
    if(Monteverdi_USE_CPACK)
      message(FATAL_ERROR "Cannot generate installer without GDAL_DATA : GDAL_DATA")
    else()
      message(WARNING "Cannot generate installer without GDAL_DATA : GDAL_DATA")
    endif()
  endif()

  # Need to include csv files provided with GDAL that contains some needed EPSG definitions
  find_path(GDAL_DATA epsg.wkt $ENV{GDAL_DATA})

  install(
    DIRECTORY $ENV{GDAL_DATA}
    DESTINATION ${APP_DATA_DIR}
    COMPONENT Resources)

  ####################### Check otb applications #######################

  if(need_apps)
    file(GLOB OTB_APPS_LIST ${OTB_MODULES_DIR}/../../../otb/applications/otbapp_*${CMAKE_SHARED_LIBRARY_SUFFIX}) # /lib/otb
    if(NOT OTB_APPS_LIST)
      message(FATAL_ERROR "No OTB-applications detected")
    endif()

    ## otb apps dir /lib/otb/applications
    install(
      DIRECTORY "${OTB_MODULES_DIR}/../../../otb/applications"
      DESTINATION ${APP_OTBLIBS_DIR}
      COMPONENT Runtime)

  endif(need_apps)

  ## directories to look for dependencies
  set(SEARCH_DIRS)
  list(APPEND SEARCH_DIRS "${QT_PLUGINS_DIR}/sqldrivers")
  list(APPEND SEARCH_DIRS "${ITK_MODULES_DIR}/../../../")
  list(APPEND SEARCH_DIRS "${OTB_MODULES_DIR}/../../../")
  list(APPEND SEARCH_DIRS "${CMAKE_INSTALL_PREFIX}/lib/otb/")

  ####################### install fixup_bundle code #######################
  ## fixup_bundle code
  if(need_apps)
  install(
    CODE
    "file(GLOB APP_LIBS \"${CMAKE_INSTALL_PREFIX}/${APP_OTBLIBS_DIR}/applications/otbapp_*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
     list(APPEND APP_LIBS \"${CMAKE_INSTALL_PREFIX}/${APP_QTPLUGINS_DIR}/sqldrivers/${APP_QTSQLITE_FILENAME}\")
     include(BundleUtilities)
     set(BU_CHMOD_BUNDLE_ITEMS ON)
     fixup_bundle(\"${APP_NAME}\" \"\${APP_LIBS}\" \"${SEARCH_DIRS}\")"
    COMPONENT ${app})

  else(need_apps)
  install(
    CODE
    "file(GLOB APP_LIBS \"${CMAKE_INSTALL_PREFIX}/${APP_QTPLUGINS_DIR}/sqldrivers/${APP_QTSQLITE_FILENAME}\")
     include(BundleUtilities)
     set(BU_CHMOD_BUNDLE_ITEMS ON)
     fixup_bundle(\"${APP_NAME}\" \"\${APP_LIBS}\" \"${SEARCH_DIRS}\")"
    COMPONENT ${app})
  endif()
endfunction(configure_app_package)



macro(create_monteverdi_application)

  cmake_parse_arguments(APPLICATION  "" "NAME;OUTPUT_NAME;COMPONENT_NAME;NEEDS_OTB_APPS" "SOURCES;LINK_LIBRARIES" ${ARGN} )

  if(Monteverdi_USE_CPACK)

    if(NOT DEFINED APPLICATION_NEEDS_OTB_APPS OR APPLICATION_NEEDS_OTB_APPS)
      set(APPLICATION_NEEDS_OTB_APPS TRUE)

    else()
      set(APPLICATION_NEEDS_OTB_APPS FALSE)
    endif()

    add_executable(${APPLICATION_NAME}
      WIN32
      MACOSX_BUNDLE
      ${APPLICATION_SOURCES})
  else()
    add_executable(${APPLICATION_NAME}
      WIN32
      ${APPLICATION_SOURCES})
  endif()

  set(EXECUTABLE_NAME ${APPLICATION_NAME})
  if(APPLE)
    string(SUBSTRING ${APPLICATION_NAME} 0 1 FIRST_LETTER)
    string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
    string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" APPLICATION_OUTPUT_NAME "${APPLICATION_NAME}")
  endif()

  if (APPLICATION_OUTPUT_NAME)
    set_target_properties(${APPLICATION_NAME} PROPERTIES OUTPUT_NAME ${APPLICATION_OUTPUT_NAME})
    set(EXECUTABLE_NAME ${APPLICATION_OUTPUT_NAME})
  endif()

  if(APPLE)
    if(Monteverdi_USE_CPACK)
      set(MACOS_FILES_DIR "${CMAKE_SOURCE_DIR}/Packaging/MacOS")

      configure_file(${MACOS_FILES_DIR}/Info.plist.in
        ${CMAKE_BINARY_DIR}/Code/Application/${APPLICATION_NAME}/Info.plist
	      @ONLY)

      configure_file(${MACOS_FILES_DIR}/StartupCommand.in
        ${CMAKE_BINARY_DIR}/Code/Application/${APPLICATION_NAME}/${EXECUTABLE_NAME}_start
	      @ONLY)

      set_target_properties(${APPLICATION_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_BINARY_DIR}/Code/Application/${APPLICATION_NAME}/Info.plist")
    endif(Monteverdi_USE_CPACK)

  endif(APPLE)

  target_link_libraries(${APPLICATION_NAME} ${APPLICATION_LINK_LIBRARIES})

#############################################################################
install(
  TARGETS ${APPLICATION_NAME}
  BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT ${APPLICATION_COMPONENT_NAME}
  RUNTIME DESTINATION ${Monteverdi_INSTALL_BIN_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  LIBRARY DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  ARCHIVE DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  )

#############################################################################

if(Monteverdi_USE_CPACK)
  configure_app_package(${APPLICATION_COMPONENT_NAME} ${APPLICATION_NEEDS_OTB_APPS})
endif(Monteverdi_USE_CPACK)


endmacro(create_monteverdi_application)