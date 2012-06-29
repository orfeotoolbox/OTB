PROJECT(PythonWrappings C)

#
# Find Perl executable

INCLUDE (${CMAKE_ROOT}/Modules/FindPerl.cmake)
IF( NOT PERL_FOUND )
  MESSAGE("Perl executable was not found")
ENDIF( NOT PERL_FOUND )
FIND_PROGRAM(PERLCXXPARSER
  NAMES ${SoftwareGuide_SOURCE_DIR}/ParseCxxExamples.pl
)


ADD_CUSTOM_TARGET(SoftwareGuideWrapping  ALL echo)

#
# At some point we should replace the manual OTB_EXAMPLES_SRCS with
# this FILE GLOB_RECURSE expression.
#
#FILE( GLOB_RECURSE OTB_EXAMPLES_SRCS  ${OTB_SOURCE_DIR}/Examples/*.cxx)

#MESSAGE("In python before ")

SET(OTB_PYTHON_EXAMPLES_SRCS
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Python/Tutorials/HelloWorld.py
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Python/Tutorials/Pipeline.py
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Python/Tutorials/FilteringPipeline.py
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Python/Tutorials/SmarterFilteringPipeline.py
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Python/Tutorials/ScalingPipeline.py
  ${OTB-Wrapping_SOURCE_DIR}/Languages/Python/Tutorials/MultiSpectral.py
#   ${OTB-Wrapping_SOURCE_DIR}/Languages/Python/Tutorials/OrthoFusion.py
)

#MESSAGE("In python after ")

#
# Parse Latex file for latex includes
#
IF( PERL_FOUND AND PERLCXXPARSER )
  FOREACH(example ${OTB_PYTHON_EXAMPLES_SRCS})
    MESSAGE("Processing python wrapping example ${example}")
    GET_FILENAME_COMPONENT(TEX_FILE ${example} NAME_WE)
    SET(TEX_FILE ${SoftwareGuide_BINARY_DIR}/Wrappings/Python/${TEX_FILE}Python.tex)
    MESSAGE("Tex file: ${TEX_FILE}")
    ADD_CUSTOM_COMMAND(
      SOURCE    ${example}
      COMMAND   ${PERL_EXECUTABLE}
      ARGS      ${PERLCXXPARSER} ${example} ${TEX_FILE}
      TARGET    SoftwareGuidePythonWrappings
      DEPENDS   ${PERLCXXPARSER} ${example}
      OUTPUTS   ${TEX_FILE}
      )
    SET(TEX_DEPENDENCIES ${TEX_DEPENDENCIES} ${TEX_FILE})
  ENDFOREACH(example)
  
ENDIF( PERL_FOUND AND PERLCXXPARSER )


ADD_CUSTOM_TARGET(BuildTexFilesPython ALL DEPENDS ${TEX_DEPENDENCIES})
SET(PYTHON_EXAMPLES_TEX_FILES ${TEX_DEPENDENCIES} CACHE INTERNAL "Internal PYTHON_EXAMPLES_TEX_FILES file list.")

