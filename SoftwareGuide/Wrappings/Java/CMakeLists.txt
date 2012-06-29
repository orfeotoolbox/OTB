PROJECT(JavaWrappings C)

#
# Find Perl executable

INCLUDE (${CMAKE_ROOT}/Modules/FindPerl.cmake)
IF( NOT PERL_FOUND )
  MESSAGE("Perl executable was not found")
ENDIF( NOT PERL_FOUND )
FIND_PROGRAM(PERLCXXPARSER
  NAMES ${SoftwareGuide_SOURCE_DIR}/ParseCxxExamples.pl
)


ADD_CUSTOM_TARGET(SoftwareGuideWrappingJava  ALL echo)

#
# At some point we should replace the manual OTB_EXAMPLES_SRCS with
# this FILE GLOB_RECURSE expression.
#
#FILE( GLOB_RECURSE OTB_EXAMPLES_SRCS  ${OTB_SOURCE_DIR}/Examples/*.cxx)


SET(OTB_JAVA_EXAMPLES_SRCS
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Java/Tutorials/HelloWorldOTB.java
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Java/Tutorials/Pipeline.java
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Java/Tutorials/FilteringPipeline.java
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Java/Tutorials/SmarterFilteringPipeline.java
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Java/Tutorials/ScalingPipeline.java
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Java/Tutorials/MultiSpectral.java
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Java/Tutorials/OrthoFusion.java
)

#
# Parse Latex file for latex includes
#
IF( PERL_FOUND AND PERLCXXPARSER )
  FOREACH(example ${OTB_JAVA_EXAMPLES_SRCS})
    MESSAGE(STATUS "Processing java wrapping example ${example}")
    GET_FILENAME_COMPONENT(TEX_FILE ${example} NAME_WE)
    SET(TEX_FILE ${SoftwareGuide_BINARY_DIR}/Wrappings/Java/${TEX_FILE}Java.tex)
    ADD_CUSTOM_COMMAND(
      SOURCE    ${example}
      COMMAND   ${PERL_EXECUTABLE}
      ARGS      ${PERLCXXPARSER} ${example} ${TEX_FILE}
      TARGET    SoftwareGuideWrappingJava
      DEPENDS   ${PERLCXXPARSER} ${example}
      OUTPUTS   ${TEX_FILE}
      )
    SET(TEX_DEPENDENCIES ${TEX_DEPENDENCIES} ${TEX_FILE})
  ENDFOREACH(example)
  
ENDIF( PERL_FOUND AND PERLCXXPARSER )


ADD_CUSTOM_TARGET(BuildTexFilesJava ALL DEPENDS ${TEX_DEPENDENCIES})
SET(JAVA_EXAMPLES_TEX_FILES ${TEX_DEPENDENCIES} CACHE INTERNAL "Internal JAVA_EXAMPLES_TEX_FILES file list.")

