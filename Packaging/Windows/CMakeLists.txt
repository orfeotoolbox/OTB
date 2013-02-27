set(CPACK_NSIS_MODIFY_PATH ON PARENT_SCOPE)

# Override the monteverdi2.exe
# with the monteverdi2.bat shortcut
set(CPACK_NSIS_MENU_LINKS
    "bin/monteverdi2.bat" "Monteverdi2" PARENT_SCOPE)

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

# Gather all dlls from which Monteverdi depends, and put them aside the executable
# For this to work, the necessary DLL must be in PATH
install(SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/BundleFixing.cmake
        COMPONENT Runtime)

# For some reason, fixup_bundle won't package the msvc runtime dll
# I guess it's because cpack already handles installing the runtime,
# but here we build with a particular version of Visual, but
# some OSGeo4W dependencies are built upon other runtimes
install(FILES ${OSGEO4W_ROOT}/bin/msvcp60.dll
              ${OSGEO4W_ROOT}/bin/msvcp70.dll
              ${OSGEO4W_ROOT}/bin/msvcp71.dll
              ${OSGEO4W_ROOT}/bin/msvcr71.dll
              ${OSGEO4W_ROOT}/bin/msvcrt.dll
        DESTINATION bin
        COMPONENT Runtime)

# Need to include csv files provided with OSGeo that contains some needed EPSG definitions
find_path(GDAL_DATA epsg.wkt $ENV{GDAL_DATA})

install(DIRECTORY ${GDAL_DATA}
        DESTINATION share
        COMPONENT Resources)

install(FILES ${QT_TRANSLATIONS_FILES}
        DESTINATION  share/qt4/translations
        COMPONENT Resources)

install(FILES qt.conf
        DESTINATION  bin
        COMPONENT Resources)
        
install(FILES ${Monteverdi2_SOURCE_DIR}/Packaging/Windows/monteverdi2.bat
        DESTINATION bin
        COMPONENT Runtime)

