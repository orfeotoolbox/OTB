option(Monteverdi2_USE_CPACK "Generate installer using CPack" OFF)
mark_as_advanced(Monteverdi2_USE_CPACK)

if( Monteverdi2_USE_CPACK )

# Get the translation files coming with Qt, and install them in the bundle
# They are loaded by Monteverdi2.
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

get_qt_translation_files(QT_TRANSLATIONS_FILES)

set(CPACK_PACKAGE_NAME "Monteverdi2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Monteverdi2")

set(CPACK_PACKAGE_VERSION  "${Monteverdi2_VERSION_STRING}")
set(CPACK_PACKAGE_VERSION_MAJOR "${Monteverdi2_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Monteverdi2_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${Monteverdi2_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT "contact@orfeo-toolbox.org" CACHE STRING "Orfeo toolbox contact email")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${Monteverdi2_SOURCE_DIR}/Description.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Monteverdi2-${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${Monteverdi2_SOURCE_DIR}/Copyright/Copyright.txt")

# http://www.vtk.org/Bug/view.php?id=7828
# Need to use the CPACK_PACKAGE_EXECUTABLES
# otherwise the shortcut in the menu will not
# appear.
#set(CPACK_PACKAGE_EXECUTABLES "monteverdi2" "Monteverdi2")

# Manage dependencies between components
set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Runtime)
set(CPACK_COMPONENT_RUNTIME_DEPENDS Resources)

set(CPACK_COMPONENT_RUNTIME_REQUIRED ON)
set(CPACK_COMPONENT_RESOURCES_REQUIRED ON)
set(CPACK_COMPONENT_RESOURCES_HIDDEN ON)

set(CPACK_COMPONENT_RUNTIME_DESCRIPTION "Monteverdi2")

# Don't package the Developement component
set(CPACK_COMPONENTS_ALL "Runtime;Resources;Unspecified")

if( APPLE )
  add_subdirectory( MacOS )
endif( APPLE )

if( WIN32 AND NOT UNIX )
  add_subdirectory( Windows )
endif( WIN32 AND NOT UNIX )

include(InstallRequiredSystemLibraries)
include(CPack)

endif()
