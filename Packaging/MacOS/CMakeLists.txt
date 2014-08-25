set(CPACK_GENERATOR "Bundle" PARENT_SCOPE)
set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Monteverdi2.icns" PARENT_SCOPE)
set(CPACK_BUNDLE_NAME "Monteverdi2-${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}" PARENT_SCOPE)
set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" PARENT_SCOPE)
set(CPACK_BUNDLE_STARTUP_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/StartupCommand" PARENT_SCOPE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)

install(DIRECTORY $ENV{GDAL_DATA}
        DESTINATION share
        COMPONENT Resources)

install(FILES ${QT_TRANSLATIONS_FILES}
        DESTINATION  share/qt4/translations
        COMPONENT Resources)

install(FILES ${QT_PLUGINS_DIR}/sqldrivers/libqsqlite.dylib
        DESTINATION lib/qt4/plugins/sqldrivers/
        COMPONENT Runtime)

install(FILES qt.conf
        DESTINATION  bin
        COMPONENT Resources)

# Search all the applications available in the OTB build dir
file(GLOB OTB_APPS_LIST ${OTB_DIR}/bin/otbapp_*.dylib)

# Install the applications 
install(FILES ${OTB_APPS_LIST}
        DESTINATION lib/otb/applications)

# Retrieve the name of all the applications available
foreach(APP ${OTB_APPS_LIST})
  string(REPLACE ${OTB_DIR}/bin/ "" APP_NAME ${APP})
  list(APPEND OTB_APPS_NAME_LIST ${APP_NAME})
endforeach(APP)

# configure the file to pass the list of available applications
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BundleFixing.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
               @ONLY)

#Use the configured file with the list of apps 
install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
        COMPONENT Runtime)
