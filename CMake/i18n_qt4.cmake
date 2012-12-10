#
# Declare and initialize variable to be present in parent scope.
# set( Monteverdi2_I18N_SOURCE_FILES
#   ""
#   CACHE INTERNAL "" FORCE
#   )
#
# Set locale human-readable translation files.
macro( add_to_qt4_i18n_sources SOURCE_FILES )
  set( Monteverdi2_I18N_SOURCE_FILES
    ${Monteverdi2_I18N_SOURCE_FILES} ${ARGV}
    CACHE INTERNAL "" FORCE
    )
endmacro()
