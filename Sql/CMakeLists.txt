###############################################################################
# SQL Queries.
#------------------------------------------------------------------------------

# SQL queries source files.
set( Monteverdi2_SQL_FILES
  sql_create_db.sql
  sql_insert_items.sql
  sql_queries.sql
)

# SQL C++ target file
set( Monteverdi2_SQL_OUTPUT
  ${CMAKE_SOURCE_DIR}/Code/Common/Core/mvdSql.cxx
)

#
# Configuration option to enable/disable ?
option(
  GENERATE_SQL
  "Generate C++ source files from SQL queries source files. WARNING: 'make clean' will delete the source ${Monteverdi2_SQL_OUTPUT} file!"
  OFF
)

if( GENERATE_SQL )
  message( AUTHOR_WARNING "WARNING: GENERATE_SQL option is ON. 'make clean' will delete the source ${Monteverdi2_SQL_OUTPUT} file!" )

  # Create source list containing absolute filenames because custom commands
  # will be called from build dir.
  foreach( SOURCE_FILE ${Monteverdi2_SQL_FILES} )
    get_filename_component( FILE ${SOURCE_FILE} ABSOLUTE )
    set( Monteverdi2_SQL_SOURCES ${Monteverdi2_SQL_SOURCES} ${FILE} )
  endforeach( SOURCE_FILE )

  # message( STATUS "SQL source files: ${Monteverdi2_SQL_SOURCES}")
  # message( STATUS "SQL output file: ${Monteverdi2_SQL_OUTPUT}")

  add_custom_target( Monteverdi2_Sql
    ALL
    #${CMAKE_SOURCE_DIR}/Scripts/genc.sh -s .sql ${Monteverdi2_SQL_SOURCES} > ${Monteverdi2_SQL_OUTPUT}
    #COMMENT "Generating ${Monteverdi2_SQL_OUTPUT}"
    DEPENDS ${Monteverdi2_SQL_OUTPUT}
  )

  add_custom_command(
    OUTPUT ${Monteverdi2_SQL_OUTPUT}
    DEPENDS ${Monteverdi2_SQL_SOURCES}
    COMMENT "Generating ${Monteverdi2_SQL_OUTPUT}"
    COMMAND ${CMAKE_SOURCE_DIR}/Scripts/genc.sh -s .sql ${Monteverdi2_SQL_SOURCES} > ${Monteverdi2_SQL_OUTPUT}
  )

  # add_custom_command(
  #   TARGET Monteverdi2_Sql
  #   PRE_BUILD
  #   COMMENT "Generating ${Monteverdi2_SQL_OUTPUT}"
  #   COMMAND "${CMAKE_SOURCE_DIR}/Scripts/genc.sh -s .sql ${Monteverdi2_SQL_SOURCES} > ${Monteverdi2_SQL_OUTPUT}"
  # )

endif( GENERATE_SQL )
