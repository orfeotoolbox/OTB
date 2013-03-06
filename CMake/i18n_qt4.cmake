#
# Declare and initialize variable to be present in parent scope.
# set( Monteverdi2_I18N_SOURCE_FILES
#   ""
#   CACHE INTERNAL "" FORCE
#   )
#
# Set locale human-readable translation files.
macro( add_to_qt4_i18n_sources SOURCE_FILES )
  
  foreach(SOURCE_FILE ${ARGV})
    get_filename_component( ABS_SOURCE_FILE ${SOURCE_FILE} ABSOLUTE )
    set( Monteverdi2_I18N_SOURCE_FILES
      ${Monteverdi2_I18N_SOURCE_FILES} ${ABS_SOURCE_FILE}
      CACHE INTERNAL "" FORCE
      )
  endforeach()
  
endmacro()
