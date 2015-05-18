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
file(GLOB OTB_APPS_LIST ${OTB_DIR}/../../otb/applications/otbapp_*.dll) # /lib/otb

if(NOT OTB_APPS_LIST)
    message(FATAL_ERROR "No OTB-applications detected")
endif()

# Install the applications
install(FILES ${OTB_APPS_LIST}
        DESTINATION lib/otb/applications
        COMPONENT Runtime)

# Retrieve the name of all the applications available
foreach(APP ${OTB_APPS_LIST})
    string(REPLACE ${OTB_DIR}/../../otb/applications/ "" APP_NAME ${APP})
    list(APPEND OTB_APPS_NAME_LIST ${APP_NAME})
endforeach(APP)

# configure the file to pass the list of available applications
set(EXE_FILENAME "monteverdi2.exe")
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/BundleFixing.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
                @ONLY)

set(EXE_FILENAME "mapla.exe")
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/BundleFixing.cmake.in
                ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing2.cmake
                @ONLY)


#Use the configured file with the list of apps
# Gather all dlls from which Monteverdi depends, and put them aside the executable
# For this to work, the necessary DLL must be in PATH
install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
                COMPONENT Monteverdi2)

install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing2.cmake
                COMPONENT Mapla)

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
        COMPONENT Monteverdi2)


install(FILES ${Monteverdi2_SOURCE_DIR}/Packaging/Windows/mapla.bat
        DESTINATION bin
        COMPONENT Mapla)
