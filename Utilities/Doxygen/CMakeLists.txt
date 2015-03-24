#
# Build the documentation
#
include (${CMAKE_ROOT}/Modules/Documentation.cmake)

mark_as_advanced(BUILD_DOCUMENTATION)

# -------------------------------------------------------------
#
# Generate Doxygen configuration file in all configuration file
#
if( ${DOXYGEN_DOT_FOUND} )
  set( OTB_HAVE_DOT "YES" )
else()
  set( OTB_HAVE_DOT "NO" )
endif()

set( DOX_MODULE_LIST ${OTB_MODULES_ENABLED} )

foreach(otb-module ${DOX_MODULE_LIST})
  if(${otb-module}_IS_TEST)
    list( APPEND DOXYGEN_TEST_DIRS "\"${${otb-module}_SOURCE_DIR}\" \\\n ")
  else()
    if( EXISTS ${${otb-module}_SOURCE_DIR}/include )
      list( APPEND DOXYGEN_INCLUDE_DIRS "\"${${otb-module}_SOURCE_DIR}/include\" \\ \n")
    endif()
  endif()
endforeach()

#list( APPEND DOXYGEN_TEST_DIRS "\"${OTB_SOURCE_DIR}/Examples\" \n")

# By definition DOXYGEN_TEST_DIRS can not be empty. In the worst case if
# BUILD_TESTING is turned OFF, there is only ${OTB_SOURCE_DIR}/Examples;
# thus we don't need to do any replacement in the string.
if( BUILD_TESTING )
  # replace ";" in the string by space+backslash+newline
  string( REPLACE ";" " \\\\n" DOXYGEN_TEST_DIRS ${DOXYGEN_TEST_DIRS} )
endif()

list( LENGTH DOXYGEN_INCLUDE_DIRS _doxygen_include_dirs_size )

if( ${_doxygen_include_dirs_size} GREATER 0 )
  # replace ";" in the string by space+backslash+newline
  string( REPLACE ";" " \\\\n" DOXYGEN_INCLUDE_DIRS ${DOXYGEN_INCLUDE_DIRS} )
endif()

#
# Configure the script and the doxyfile, then add target
#

set( OTB_DOXYGEN_OUTPUT_DIR ${OTB_BINARY_DIR}/Documentation/Doxygen )
file(MAKE_DIRECTORY ${OTB_DOXYGEN_OUTPUT_DIR})

set( OTB_DOXYGEN_INPUT ${DOXYGEN_INCLUDE_DIRS} )

SET( OTB_DOXYGEN_DIAGRAMS YES )

option( OTB_DOXYGEN_HTML "Doxygen will generate HTML Output" ON )
option( OTB_DOXYGEN_DOCSET "Doxygen will generate additional index files for Xcode 3" OFF )
option( OTB_DOXYGEN_CHM "Doxygen will generate Microsoft HTML help" OFF )
option( OTB_DOXYGEN_QHP "Doxygen will generate Qt Compressed Help" OFF )
option( OTB_DOXYGEN_ECLIPSEHELP "Doxygen will generate Eclipse help plugin" OFF )
option( OTB_DOXYGEN_LATEX "Doxygen will generate LaTeX output" OFF )
option( OTB_DOXYGEN_RTF "Doxygen will generate RTF output optimized for Word 97" OFF )
option( OTB_DOXYGEN_XML "Doxygen will generate XML output" OFF )

mark_as_advanced( OTB_DOXYGEN_HTML
  OTB_DOXYGEN_DOCSET
  OTB_DOXYGEN_CHM
  OTB_DOXYGEN_QHP
  OTB_DOXYGEN_ECLIPSEHELP
  OTB_DOXYGEN_LATEX
  OTB_DOXYGEN_RTF
  OTB_DOXYGEN_XML
)

set( OTB_DOXYGEN_OUTPUT
  HTML
  DOCSET
  CHM
  QHP
  ECLIPSEHELP
  LATEX
  RTF
  XML
)

foreach( var ${OTB_DOXYGEN_OUTPUT} )
  if( OTB_DOXYGEN_${var} )
    set( OTB_DOXYGEN_GEN_${var} "YES" )
  else()
    set( OTB_DOXYGEN_GEN_${var} "NO" )
  endif()
endforeach()

set(OTB_DOXYGEN_ITK_TAGFILE "" CACHE FILEPATH "Tagfile containing ITK doxygen")
set(OTB_DOXYGEN_ITK_DOXYGEN_URL "" CACHE STRING "URL to ITK doxygen")

if(("${OTB_DOXYGEN_ITK_TAGFILE}" STREQUAL "") OR ("${OTB_DOXYGEN_ITK_DOXYGEN_URL}" STREQUAL ""))
  set(OTB_DOXYGEN_TAGFILE_FIELD "")
else()
  set(OTB_DOXYGEN_TAGFILE_FIELD "${OTB_DOXYGEN_ITK_TAGFILE}=${OTB_DOXYGEN_ITK_DOXYGEN_URL}")
endif()

find_package( Perl )

if( PERL_FOUND )
  set( OTB_DOXYGEN_INPUT_FILTER
    "${PERL_EXECUTABLE} ${OTB_BINARY_DIR}/Utilities/Doxygen/otbdoxygen.pl" )

  configure_file(${OTB_SOURCE_DIR}/Utilities/Doxygen/otbdoxygen.pl.in
               ${OTB_BINARY_DIR}/Utilities/Doxygen/otbdoxygen.pl)
else()
  set( OTB_DOXYGEN_INPUT_FILTER )
endif()

configure_file(${OTB_SOURCE_DIR}/Utilities/Doxygen/doxygen.config.in
               ${OTB_BINARY_DIR}/Utilities/Doxygen/doxygen.config)


#set( OTB_DOXYGEN_OUTPUT_DIR ${OTB_BINARY_DIR}/temp )

set( OTB_DOXYGEN_INPUT "\"INPUT_SINGLE_FILE\"" )

set( OTB_DOXYGEN_DIAGRAMS NO )


configure_file(${OTB_SOURCE_DIR}/Utilities/Doxygen/doxygen.config.in
               ${OTB_BINARY_DIR}/single-doxygen.config)

# -------------------------------------------------------------
if (BUILD_DOCUMENTATION)

  find_package( LATEX )

  if( NOT LATEX_COMPILER )
    message( "Warning: LaTeX not found. Formulas will not be generated in documentation." )
  endif()

  # Custom command to generate a examples page which include all OTB examples
  #add_custom_command( OUTPUT "${OTB_BINARY_DIR}/Documentation/Doxygen/Examples.dox"
  #  COMMAND ${CMAKE_COMMAND} -D "PROJECT_SOURCE_DIR:PATH=${OTB_SOURCE_DIR}"
  #  -D "OUTPUT_FILE:PATH=${OTB_BINARY_DIR}/Documentation/Doxygen/Examples.dox"
  #  -P "${OTB_SOURCE_DIR}/Utilities/Doxygen/GenerateExamplesDox.cmake"
  #  WORKING_DIRECTORY "${OTB_SOURCE_DIR}/Examples"
  #  DEPENDS "${OTB_SOURCE_DIR}/Examples" "${OTB_SOURCE_DIR}/Utilities/Doxygen/GenerateExamplesDox.cmake"
  #  )

  add_custom_target(Documentation ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${OTB_BINARY_DIR}/Utilities/Doxygen/doxygen.config
    MAIN_DEPENDENCY ${OTB_BINARY_DIR}/Utilities/Doxygen/doxygen.config
    DEPENDS         ${OTB_BINARY_DIR}/Utilities/Doxygen/otbdoxygen.pl
   # DEPENDS         ${OTB_BINARY_DIR}/Documentation/Doxygen/Examples.dox
    WORKING_DIRECTORY ${OTB_BINARY_DIR}/Utilities/Doxygen
    )

  # Somehow the logo image is not copied into the html folder.
  # Here we ensure that the logo image ends up in the right directory
  # where html pages could find it.



  message( STATUS
           "To generate Doxygen's documentation, you need to build the Documentation target"
         )

endif (BUILD_DOCUMENTATION)
