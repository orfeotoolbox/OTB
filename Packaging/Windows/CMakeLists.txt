set(CPACK_NSIS_MODIFY_PATH ON PARENT_SCOPE)

# Override the monteverdi2.exe
# with the monteverdi2.bat shortcut
set(CPACK_NSIS_MENU_LINKS
    "bin/monteverdi2.bat" "Monteverdi2"
    PARENT_SCOPE)

# Since we put bat files as MENU_LINKS, they does not get an icon automatically
# though NSIS can do that.
# That's why we use the following hack to associate the icon coming from the executable
# to the bat file :
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
    "
    CreateShortCut \\\"$SMPROGRAMS\\\\Monteverdi2-${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}\\\\Monteverdi2.lnk\\\" \\\"$INSTDIR\\\\bin\\\\monteverdi2.bat\\\" \\\" \\\" \\\"$INSTDIR\\\\bin\\\\monteverdi2.exe\\\"
    "
    PARENT_SCOPE)

set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
    "
    Delete \\\"$SMPROGRAMS\\\\Monteverdi2-${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}\\\\Monteverdi2.lnk\\\"
    "
    PARENT_SCOPE)

# Icon for the add-remove program
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\monteverdi2.exe" PARENT_SCOPE)

# set the install/unistall icon used for the installer itself
# There is a bug in NSI that does not handle full unix paths properly.
set(CPACK_NSIS_MUI_ICON    "${Monteverdi2_SOURCE_DIR}/Data/Icons/monteverdi2.ico" PARENT_SCOPE)
set(CPACK_NSIS_MUI_UNIICON "${Monteverdi2_SOURCE_DIR}/Data/Icons/monteverdi2.ico" PARENT_SCOPE)


file(TO_CMAKE_PATH "$ENV{OSGEO4W_ROOT}" OSGEO4W_ROOT)
if(NOT OSGEO4W_ROOT)
  message(FATAL_ERROR "Cannot generate installer without OSGeo4W environment : OSGEO4W_ROOT")
endif(NOT OSGEO4W_ROOT)

file(TO_CMAKE_PATH "$ENV{GDAL_DATA}" GDAL_DATA)
if(NOT GDAL_DATA)
  message(FATAL_ERROR "Cannot generate installer without OSGeo4W environment : GDAL_DATA")
endif(NOT GDAL_DATA)


# Search all the applications available in the OTB install dir
# Consequently, this part of code only works with an OTB install dir,
# not with an OB build dir.
file(GLOB OTB_APPS_LIST ${OTB_DIR}/applications/otbapp_*.dll) # /lib/otb

if(NOT OTB_APPS_LIST)
    message(FATAL_ERROR "No OTB-applications detected")
endif()

# Install the applications 
install(FILES ${OTB_APPS_LIST}
        DESTINATION lib/otb/applications
        COMPONENT Runtime)

# Retrieve the name of all the applications available
foreach(APP ${OTB_APPS_LIST})
    string(REPLACE ${OTB_DIR}/applications/ "" APP_NAME ${APP})
    list(APPEND OTB_APPS_NAME_LIST ${APP_NAME})
endforeach(APP)

# configure the file to pass the list of available applications
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/BundleFixing.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
                @ONLY)

#Use the configured file with the list of apps 
# Gather all dlls from which Monteverdi depends, and put them aside the executable
# For this to work, the necessary DLL must be in PATH
install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
                COMPONENT Runtime)

# The following DLL handling is very specific
# both to OSGeo4W flavour (32 bit or 64 bit), OSGeo4W package state
# and the compiler used to make the Monteverdi2 installer (VC2010)
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
     install( FILES ${dll}
              DESTINATION bin
              COMPONENT Runtime )
  endif()
endforeach()


# Need to include csv files provided with OSGeo that contains some needed EPSG definitions
find_path(GDAL_DATA epsg.wkt $ENV{GDAL_DATA})

install(DIRECTORY ${GDAL_DATA}
        DESTINATION share
        COMPONENT Resources)

install(FILES ${QT_TRANSLATIONS_FILES}
        DESTINATION  share/qt4/translations
        COMPONENT Resources)
        
install(FILES ${QT_PLUGINS_DIR}/sqldrivers/qsqlite4.dll
        DESTINATION lib/qt4/plugins/sqldrivers/
        COMPONENT Runtime)

install(FILES qt.conf
        DESTINATION  bin
        COMPONENT Resources)
        
install(FILES ${Monteverdi2_SOURCE_DIR}/Packaging/Windows/monteverdi2.bat
        DESTINATION bin
        COMPONENT Runtime)

