set(APPLICATION_NAME "Monteverdi2")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in 
                      ${CMAKE_BINARY_DIR}/Packaging/MacOS/Monteverdi2-Info.plist)

set(EXE_FILENAME "monteverdi2")                      
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/StartupCommand.in 
                      ${CMAKE_BINARY_DIR}/Packaging/MacOS/Monteverdi2-StartupCommand
                      @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BundleFixing.cmake.in
                      ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
                      @ONLY)

#mapla                      
set(APPLICATION_NAME "Mapla")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in 
                      ${CMAKE_BINARY_DIR}/Packaging/MacOS/Mapla-Info.plist)

set(EXE_FILENAME "mapla")                      
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/StartupCommand.in 
                      ${CMAKE_BINARY_DIR}/Packaging/MacOS/Mapla-StartupCommand
                      @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BundleFixing.cmake.in
                      ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing2.cmake
                      @ONLY)

               
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
  DESTINATION lib/otb/applications
  COMPONENT Runtime)

# Retrieve the name of all the applications available
foreach(APP ${OTB_APPS_LIST})
  string(REPLACE ${OTB_DIR}/bin/ "" APP_NAME ${APP})
  list(APPEND OTB_APPS_NAME_LIST ${APP_NAME})
endforeach(APP)


#Use the configured file with the list of apps : monteverdi2
install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing.cmake
        COMPONENT Monteverdi2)

        
#Use the configured file with the list of apps : mapla
install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/BundleFixing2.cmake
        COMPONENT Mapla)        
