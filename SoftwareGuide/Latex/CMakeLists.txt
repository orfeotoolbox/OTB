
#
# Find LaTeX
#

INCLUDE (${CMAKE_ROOT}/Modules/FindLATEX.cmake)

FIND_PROGRAM(TEX4HT_COMPILER
  NAMES htlatex
  PATHS ${MIKTEX_BINARY_PATH}
        /usr/bin
)
MARK_AS_ADVANCED(TEX4HT_COMPILER)


FIND_PROGRAM(SH_COMMAND
  NAMES sh
  PATHS /usr/bin
)
MARK_AS_ADVANCED(SH_COMMAND)


FIND_PROGRAM(TAR_COMMAND
  NAMES tar
  PATHS /usr/bin
)
MARK_AS_ADVANCED(TAR_COMMAND)




#
# Verify that we found all the components we need
#
IF(NOT LATEX_COMPILER)
  MESSAGE(FATAL_ERROR "Latex compiler was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT LATEX_COMPILER)

IF(NOT BIBTEX_COMPILER)
  MESSAGE(FATAL_ERROR "BibTeX executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT BIBTEX_COMPILER)

IF(NOT DVIPS_CONVERTER)
  MESSAGE(FATAL_ERROR "dvips executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT DVIPS_CONVERTER)

IF(NOT PS2PDF_CONVERTER)
  MESSAGE(FATAL_ERROR "ps2pdf executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT PS2PDF_CONVERTER)

IF(NOT MAKEINDEX_COMPILER)
  MESSAGE(FATAL_ERROR "makeindex executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT MAKEINDEX_COMPILER)

IF(NOT TEX4HT_COMPILER)
  MESSAGE(FATAL_ERROR "htlatex executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT TEX4HT_COMPILER)


#
# Options for PDF generation.
# resolution in dpi.
#
#  /screen   = low    resolution = Distiller "Screen" optimized
#  /ebook    = medium resolution = Distiller "eBook"
#  /printer  = high   resolution = Distiller "Print" optimized
#  /prepress = high   resolution = Distiller like "Print" with some error checking differences
#
#SET(PS2PDF_CONVERTER_OPTIONS "-dPDFSETTINGS=/printer" -r600)
SET(PS2PDF_CONVERTER_OPTIONS "-dPDFSETTINGS=/prepress" "-dColorImageResolution=600" "-dGrayImageResolution=600" -r600)
#SET(PS2PDF_CONVERTER_OPTIONS "-dPDFSETTINGS=/screen" -r600)

SET(DVIPS_CONVERTER_OPTIONS  -D600)


# Option for html generation
SET(TEX4HT_OPTIONS html,2,sections+)
SET (HTML_OUTPUT "SoftwareGuide-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")

#
#copy release_notes from source tree
#
SET (RELEASE_NOTES_SRC RELEASE_NOTES.txt)
#TODO transform the copy as a custom command
   #ADD_CUSTOM_COMMAND(
    #    PRE_BUILD
     #   OUTPUT  ${SoftwareGuide_SOURCE_DIR}/Latex/${RELEASE_NOTES_SRC}
      #  COMMAND ${CMAKE_COMMAND} -E copy ${OTB_SOURCE_DIR}/${RELEASE_NOTES_SRC}  ${SoftwareGuide_SOURCE_DIR}/Latex/${RELEASE_NOTES_SRC}
       # COMMENT   "Copying the Release Notes"
   #)
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy "${OTB_SOURCE_DIR}/${RELEASE_NOTES_SRC}" "${SoftwareGuide_SOURCE_DIR}/Latex/${RELEASE_NOTES_SRC}")
   
#
# File that defines the path to .cxx examples in ITK sources
#
CONFIGURE_FILE(
${SoftwareGuide_SOURCE_DIR}/SoftwareGuideConfiguration.tex.in
${SoftwareGuide_BINARY_DIR}/SoftwareGuideConfiguration.tex
)

SET( Tex_SRCS
  Abstract.tex
  Applications.tex
  AnisotropicDiffusionFiltering.tex
  Contributors.tex
  DataRepresentation.tex
  Filtering.tex
  GUI.tex
  ImageMetrics.tex
  ImageInterpolators.tex
  ImageAdaptors.tex
  Infrastructure.tex
  Installation.tex
  Introduction.tex
  IO.tex
  Iterators.tex
  Numerics.tex
  Registration.tex
  StereoReconstruction.tex
  Segmentation.tex
  SoftwareGuide.tex
  SoftwareProcess.tex
  SpatialObjects.tex
  Statistics.tex
  SystemOverview.tex
  Transforms.tex
  Visualization.tex
  Watersheds.tex
  Fusion.tex
  Radiometry.tex
  FeatureExtraction.tex
  ObjectBasedImageAnalysis.tex
  Hyperspectral.tex
  ReleaseNotes.tex
)

IF(GENERATE_WRAPPING_DOC)
  SET(Tex_SRCS   ${Tex_SRCS} Wrapping.tex)
ENDIF(GENERATE_WRAPPING_DOC)
  


FOREACH( file ${Tex_SRCS})
MESSAGE(STATUS "Processing File " ${file})
SET( SoftwareGuide_Tex_SRCS
   ${SoftwareGuide_Tex_SRCS} ${SoftwareGuide_SOURCE_DIR}/Latex/${file}
 )
ENDFOREACH( file )

FOREACH( exampletexname ${EXAMPLES_TEX_FILES})
  SET( ITK_EXAMPLES_TEX_SRCS ${ITK_EXAMPLES_TEX_SRCS} ${exampletexname})
ENDFOREACH( exampletexname )

FOREACH( exampletexname ${JAVA_EXAMPLES_TEX_FILES})
  SET( ITK_EXAMPLES_TEX_SRCS ${ITK_EXAMPLES_TEX_SRCS} ${exampletexname})
ENDFOREACH( exampletexname )

#Generate random contributor list
MESSAGE(STATUS "Generating contributors list")
EXECUTE_PROCESS(COMMAND python  generateRandomContributorList.py
                WORKING_DIRECTORY ${SoftwareGuide_SOURCE_DIR}/Latex/ )
MESSAGE(STATUS "Generating contributors list done")

  IF (NOT UNIX)
    SET(ITK_TEXINPUTS ${SoftwareGuide_SOURCE_DIR}/../Latex;${SoftwareGuide_SOURCE_DIR};${SoftwareGuide_SOURCE_DIR}/Latex;${SoftwareGuide_SOURCE_DIR}/Art;${SoftwareGuide_BINARY_DIR};${SoftwareGuide_BINARY_DIR}/Examples;${SoftwareGuide_BINARY_DIR}/Art;${SoftwareGuide_BINARY_DIR}/Latex;${SoftwareGuide_BINARY_DIR}/Wrappings/Java;${SoftwareGuide_BINARY_DIR}/Wrappings/Python)

    CONFIGURE_FILE(${SoftwareGuide_SOURCE_DIR}/LaTeXWrapper.bat.in
                   ${SoftwareGuide_BINARY_DIR}/LaTeXWrapper.bat)
    SET (LATEX_WRAPPER1 ${SoftwareGuide_BINARY_DIR}/LaTeXWrapper.bat)
    SET (LATEX_WRAPPER2 "")

    CONFIGURE_FILE(${SoftwareGuide_SOURCE_DIR}/DvipsWrapper.bat.in
                   ${SoftwareGuide_BINARY_DIR}/DvipsWrapper.bat)
    SET (DVIPS_WRAPPER1 ${SoftwareGuide_BINARY_DIR}/DvipsWrapper.bat)
    SET (DVIPS_WRAPPER2 "")
    IF (TEX4HT_COMPILER)
      CONFIGURE_FILE(${SoftwareGuide_SOURCE_DIR}/Tex4htWrapper.bat.in
                   ${SoftwareGuide_BINARY_DIR}/Tex4htWrapper.bat)
      SET (LATEX_WRAPPER1 ${SoftwareGuide_BINARY_DIR}/Tex4htWrapper.bat)
      SET (LATEX_WRAPPER2 "")
    ENDIF (TEX4HT_COMPILER)
  ELSE (NOT UNIX)
    SET(ITK_TEXINPUTS ${SoftwareGuide_SOURCE_DIR}/../Latex:${SoftwareGuide_SOURCE_DIR}:${SoftwareGuide_SOURCE_DIR}/Latex:${SoftwareGuide_SOURCE_DIR}/Art:${SoftwareGuide_BINARY_DIR}:${SoftwareGuide_BINARY_DIR}/Examples:${SoftwareGuide_BINARY_DIR}/Art:${SoftwareGuide_BINARY_DIR}/Latex:${SoftwareGuide_BINARY_DIR}/Wrappings/Java:${SoftwareGuide_BINARY_DIR}/Wrappings/Python)
    
    CONFIGURE_FILE(${SoftwareGuide_SOURCE_DIR}/LaTeXWrapper.sh.in
                   ${SoftwareGuide_BINARY_DIR}/LaTeXWrapper.sh)
    SET (LATEX_WRAPPER1 /bin/sh)
    SET (LATEX_WRAPPER2 ${SoftwareGuide_BINARY_DIR}/LaTeXWrapper.sh)

    CONFIGURE_FILE(${SoftwareGuide_SOURCE_DIR}/DvipsWrapper.sh.in
                   ${SoftwareGuide_BINARY_DIR}/DvipsWrapper.sh)
    SET (DVIPS_WRAPPER1 /bin/sh)
    SET (DVIPS_WRAPPER2 ${SoftwareGuide_BINARY_DIR}/DvipsWrapper.sh)
    IF (TEX4HT_COMPILER)
      CONFIGURE_FILE(${SoftwareGuide_SOURCE_DIR}/Tex4htWrapper.sh.in
                   ${SoftwareGuide_BINARY_DIR}/Tex4htWrapper.sh)
      SET (TEX4HT_WRAPPER1 ${SoftwareGuide_BINARY_DIR}/Tex4htWrapper.sh)
      SET (TEX4HT_WRAPPER2 "")
    ENDIF (TEX4HT_COMPILER)
  ENDIF (NOT UNIX)

  ADD_CUSTOM_COMMAND(
    OUTPUT    SoftwareGuide.pdf
    COMMAND   ${LATEX_WRAPPER1} ${LATEX_WRAPPER2} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.tex
    COMMAND   ${MAKEINDEX_COMPILER} SoftwareGuide
    COMMAND   ${BIBTEX_COMPILER} SoftwareGuide
    COMMAND   ${LATEX_WRAPPER1} ${LATEX_WRAPPER2} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.tex
    COMMAND   ${LATEX_WRAPPER1} ${LATEX_WRAPPER2} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.tex
    COMMAND   ${DVIPS_WRAPPER1} ${DVIPS_WRAPPER2} SoftwareGuide.dvi ${DVIPS_CONVERTER_OPTIONS} -o  SoftwareGuide.ps
    COMMAND   ${PS2PDF_CONVERTER} ${PS2PDF_CONVERTER_OPTIONS} SoftwareGuide.ps
    DEPENDS   ${SoftwareGuide_Tex_SRCS} ${ITK_EXAMPLES_TEX_SRCS}
    COMMENT   "Generating pdf file"
  )

ADD_CUSTOM_TARGET(SoftwareGuide_Pdf ALL DEPENDS SoftwareGuide.pdf)

# Rename the SoftwareGuide.pdf file into SoftwareGuide-version.pdf
SET(FINAL_DOC_NAME "${SoftwareGuide_BINARY_DIR}/Latex/SoftwareGuide-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}")
ADD_CUSTOM_TARGET(SoftwareGuide_RenamePdf
                  ALL
   		  ${CMAKE_COMMAND} -E rename ${SoftwareGuide_BINARY_DIR}/Latex/SoftwareGuide.pdf ${FINAL_DOC_NAME}.pdf
                  COMMENT "Renaming the pdf document" 
       )
ADD_DEPENDENCIES(SoftwareGuide_RenamePdf SoftwareGuide_Pdf)

ADD_CUSTOM_TARGET(SoftwareGuide_HTML
		  ALL
		  COMMAND ${SH_COMMAND} ${SoftwareGuide_BINARY_DIR}/Tex4htWrapper.sh SoftwareGuide
		  WORKING_DIRECTORY ${SoftwareGuide_BINARY_DIR}
		  COMMENT "Building HTML documentation"
		  )
ADD_DEPENDENCIES(SoftwareGuide_HTML SoftwareGuide_RenamePdf)

ADD_CUSTOM_TARGET(SoftwareGuide_HTML_index
		  ALL
		  COMMAND rm -f index.html
		  COMMAND ln -s SoftwareGuide.html index.html
		  WORKING_DIRECTORY ${SoftwareGuide_BINARY_DIR}/SoftwareGuide-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}
		  COMMENT "Building HTML documentation index.html"
		  )
ADD_DEPENDENCIES(SoftwareGuide_HTML_index SoftwareGuide_HTML)


ADD_CUSTOM_TARGET(SoftwareGuide_HTML_targz
		  ALL
		  COMMAND ${TAR_COMMAND} cvzf SoftwareGuide-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}-html.tar.gz SoftwareGuide-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}
		  WORKING_DIRECTORY ${SoftwareGuide_BINARY_DIR}
		  COMMENT "Building Software Guide HTML documentation tar.gz"
		  )
ADD_DEPENDENCIES(SoftwareGuide_HTML_targz SoftwareGuide_HTML_index)


# FAQ

CONFIGURE_FILE(
${SoftwareGuide_SOURCE_DIR}/generate_FAQ.sh.in
${SoftwareGuide_BINARY_DIR}/generate_FAQ.sh
IMMEDIATE
)

ADD_CUSTOM_COMMAND(
  OUTPUT    OTB-FAQ.pdf
  COMMAND   ${SH_COMMAND} ${SoftwareGuide_BINARY_DIR}/generate_FAQ.sh
#  DEPENDS   SoftwareGuide.pdf
  COMMENT   "Generating FAQ"
)

ADD_CUSTOM_TARGET(FAQ ALL DEPENDS OTB-FAQ.pdf)

ADD_CUSTOM_TARGET(FAQ_HTML_targz
		  ALL
		  COMMAND ${TAR_COMMAND} cvzf OTB-FAQ-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}-html.tar.gz OTB-FAQ-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}
		  WORKING_DIRECTORY ${SoftwareGuide_BINARY_DIR}
		  COMMENT "Building FAQ HTML documentation tar.gz"
		  )
ADD_DEPENDENCIES(FAQ_HTML_targz  FAQ)


