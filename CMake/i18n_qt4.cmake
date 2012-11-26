#
# Set locale human-readable translation files.
macro( add_to_qt4_i18n_sources SOURCES)
  message( STATUS "Added I18N sources:'${ARGV}'" )
  set( i18n_SOURCES ${i18n_SOURCES} ${ARGV} )
endmacro()
