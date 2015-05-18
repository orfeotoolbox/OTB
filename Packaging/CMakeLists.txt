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



if( APPLE )
  add_subdirectory( MacOS )
endif( APPLE )

if( WIN32)
  add_subdirectory( Windows )
  endif( WIN32 )


 #Resources;Runtime; and Monteverdi are appended to
#CPACK_COMPONENTS_ALL  in create_cpack_config()
# SET(CPACK_COMPONENTS_ALL)
# LIST(APPEND CPACK_COMPONENTS_ALL  Resources)
# LIST(APPEND CPACK_COMPONENTS_ALL Runtime)
# LIST(APPEND CPACK_COMPONENTS_ALL ${application})
 if(WIN32 OR APPLE)
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Monteverdi2")
  create_cpack_config(Monteverdi2)

  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Mapla - Monteverdi2 Application Launcher")
  create_cpack_config(Mapla)
endif()

endif()
