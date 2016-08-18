#
# Reset Qt I18N source files cache variable.
macro( reset_qt4_i18n_sources )
  set( OTB_QT_I18N_SOURCE_FILES "" CACHE INTERNAL "" FORCE )
endmacro()

#
# Add source files to Qt I18n translation build.
macro( add_to_qt4_i18n_sources SOURCE_FILES )
  foreach( SOURCE_FILE ${ARGV} )
    get_filename_component( ABS_SOURCE_FILE ${SOURCE_FILE} ABSOLUTE )

    set( OTB_QT_I18N_SOURCE_FILES
      ${OTB_QT_I18N_SOURCE_FILES} ${ABS_SOURCE_FILE}
      CACHE INTERNAL "" FORCE
      )
  endforeach()

endmacro()
