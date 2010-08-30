
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

#
# Verify that we found all the components we need
#
IF(NOT LATEX_COMPILER)
  MESSAGE("Latex compiler was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT LATEX_COMPILER)

IF(NOT BIBTEX_COMPILER)
  MESSAGE("BibTeX executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT BIBTEX_COMPILER)

IF(NOT DVIPS_CONVERTER)
  MESSAGE("dvips executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT DVIPS_CONVERTER)

IF(NOT PS2PDF_CONVERTER)
  MESSAGE("ps2pdf executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT PS2PDF_CONVERTER)

IF(NOT MAKEINDEX_COMPILER)
  MESSAGE("makeindex executable was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT MAKEINDEX_COMPILER)

IF(NOT TEX4HT_COMPILER)
  MESSAGE("htlatex executable was not found. Please pass to advanced mode and provide its full path")
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
SET (HTML_OUTPUT "Html")

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
EXEC_PROGRAM(${CMAKE_COMMAND} ARGS -E copy "\"${OTB_SOURCE_DIR}/${RELEASE_NOTES_SRC}\"" "\"${SoftwareGuide_SOURCE_DIR}/Latex/${RELEASE_NOTES_SRC}\"")
   
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
  ReleaseNotes.tex
)

IF(GENERATE_WRAPPING_DOC)
  SET(Tex_SRCS   ${Tex_SRCS} Wrapping.tex)
ENDIF(GENERATE_WRAPPING_DOC)
  


FOREACH( file ${Tex_SRCS})
MESSAGE("Processing File " ${file})
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



IF(LATEX_COMPILER)

  MESSAGE("Have latex compiler")

  IF (NOT UNIX)
    #SET(ITK_TEXINPUTS ${SoftwareGuide_SOURCE_DIR}/../Latex;${SoftwareGuide_SOURCE_DIR};${SoftwareGuide_SOURCE_DIR}/Latex;${SoftwareGuide_SOURCE_DIR}/Art;${SoftwareGuide_BINARY_DIR};${SoftwareGuide_BINARY_DIR}/Examples;${SoftwareGuide_BINARY_DIR}/Wrappings/Java;${SoftwareGuide_BINARY_DIR}/Art;${SoftwareGuide_BINARY_DIR}/Latex)
    
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
    #SET(ITK_TEXINPUTS ${SoftwareGuide_SOURCE_DIR}/../Latex:${SoftwareGuide_SOURCE_DIR}:${SoftwareGuide_SOURCE_DIR}/Latex:${SoftwareGuide_SOURCE_DIR}/Art:${SoftwareGuide_BINARY_DIR}:${SoftwareGuide_BINARY_DIR}/Examples:${SoftwareGuide_BINARY_DIR}/Wrappings/Java:${SoftwareGuide_BINARY_DIR}/Art:${SoftwareGuide_BINARY_DIR}/Latex)
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
    SOURCE    ${SoftwareGuide_Tex_SRCS}  ${ITK_EXAMPLES_TEX_SRCS}
    COMMAND   ${LATEX_WRAPPER1}
    ARGS      ${LATEX_WRAPPER2}
              ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.tex
    DEPENDS   ${SoftwareGuide_Tex_SRCS} ${ITK_EXAMPLES_TEX_SRCS}
    OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.dvi
    COMMENT   "Generating dvi file"
  )
  SET(TEX_OUTPUTS ${TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.dvi)
  
  IF(DVIPS_CONVERTER)
    MESSAGE("DVIPS converter")
    ADD_CUSTOM_COMMAND(
      SOURCE    SoftwareGuide.dvi
      COMMAND   ${DVIPS_WRAPPER1}
      ARGS      ${DVIPS_WRAPPER2}
                SoftwareGuide.dvi ${DVIPS_CONVERTER_OPTIONS} -o  SoftwareGuide.ps
      DEPENDS   SoftwareGuide.ind SoftwareGuide.bbl SoftwareGuide.dvi
      OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.ps
      COMMENT   "Generating ps file"
    )
  SET(TEX_OUTPUTS ${TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.ps)

  ENDIF(DVIPS_CONVERTER)

  IF(PS2PDF_CONVERTER)
    ADD_CUSTOM_COMMAND(
      SOURCE    SoftwareGuide.ps
      COMMAND   ${PS2PDF_CONVERTER}
      ARGS      ${PS2PDF_CONVERTER_OPTIONS} SoftwareGuide.ps
      OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.pdf
      COMMENT   "Generating pdf file"
    )
    SET(TEX_OUTPUTS ${TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.pdf)
  ENDIF(PS2PDF_CONVERTER)

  IF(MAKEINDEX_COMPILER)
    ADD_CUSTOM_COMMAND(
      SOURCE    ${SoftwareGuide_Tex_SRCS}  ${ITK_EXAMPLES_TEX_SRCS}
      COMMAND   ${LATEX_WRAPPER1}
      ARGS      ${LATEX_WRAPPER2}
                ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.tex
      OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.idx
      COMMENT   "Generating idx file"
    )
  SET(TEX_OUTPUTS ${TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.idx)
  
    ADD_CUSTOM_COMMAND(
      SOURCE    SoftwareGuide.idx
      COMMAND   ${MAKEINDEX_COMPILER}
      ARGS      SoftwareGuide
      OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.ind
      COMMENT   "Generating ind file"
    )
  SET(TEX_OUTPUTS ${TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.ind)
  ENDIF(MAKEINDEX_COMPILER)

  IF(BIBTEX_COMPILER)
    ADD_CUSTOM_COMMAND(
      SOURCE    SoftwareGuide.aux
      COMMAND   ${BIBTEX_COMPILER}
      ARGS      SoftwareGuide
      OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.bbl
      COMMENT   "Generating bbl file"
    )
  SET(TEX_OUTPUTS ${TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/SoftwareGuide.bbl)
  ENDIF(BIBTEX_COMPILER)


  IF (GENERATE_MONTEVERDI_DOC)
  
    SET ( Monteverdi_Tex_SRCS 
      ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.tex
    )
    
    #tex
    ADD_CUSTOM_COMMAND(
    SOURCE    ${MonteverdiGuide_Tex_SRCS}  ${ITK_EXAMPLES_TEX_SRCS}
    COMMAND   ${LATEX_WRAPPER1}
    ARGS      ${LATEX_WRAPPER2}
              ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.tex
    DEPENDS   ${MonteverdiGuide_Tex_SRCS} ${ITK_EXAMPLES_TEX_SRCS}
    OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.dvi
    COMMENT   "Generating Monteverdi user guide dvi file"
  )
  SET(Monteverdi_TEX_OUTPUTS ${Monteverdi_TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.dvi)
  
  IF(DVIPS_CONVERTER)
    MESSAGE("DVIPS converter")
    ADD_CUSTOM_COMMAND(
      SOURCE    MonteverdiGuide.dvi
      COMMAND   ${DVIPS_WRAPPER1}
      ARGS      ${DVIPS_WRAPPER2}
                MonteverdiGuide.dvi ${DVIPS_CONVERTER_OPTIONS} -o  MonteverdiGuide.ps
      #DEPENDS   MonteverdiGuide.ind MonteverdiGuide.bbl MonteverdiGuide.dvi
      OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.ps
      COMMENT   "Generating Monteverdi user guide ps file"
    )
  SET(Monteverdi_TEX_OUTPUTS ${Monteverdi_TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.ps)

  ENDIF(DVIPS_CONVERTER)

  IF(PS2PDF_CONVERTER)
    ADD_CUSTOM_COMMAND(
      SOURCE    MonteverdiGuide.ps
      COMMAND   ${PS2PDF_CONVERTER}
      ARGS      ${PS2PDF_CONVERTER_OPTIONS} MonteverdiGuide.ps
      OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.pdf
      COMMENT   "Generating Monteverdi user guide pdf file"
    )
    SET(Monteverdi_TEX_OUTPUTS ${Monteverdi_TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.pdf)
  ENDIF(PS2PDF_CONVERTER)

#   IF(MAKEINDEX_COMPILER)
#     ADD_CUSTOM_COMMAND(
#       SOURCE    ${MonteverdiGuide_Tex_SRCS}  ${ITK_EXAMPLES_TEX_SRCS}
#       COMMAND   ${LATEX_WRAPPER1}
#       ARGS      ${LATEX_WRAPPER2}
#                 ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.tex
#       OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.idx
#       COMMENT   "Generating Monteverdi idx file"
#     )
#   SET(Monteverdi_TEX_OUTPUTS ${Monteverdi_TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.idx)
#   
#     ADD_CUSTOM_COMMAND(
#       SOURCE    MonteverdiGuide.idx
#       COMMAND   ${MAKEINDEX_COMPILER}
#       ARGS      MonteverdiGuide
#       OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.ind
#       COMMENT   "Generating Monteverdi ind file"
#     )
#   SET(Monteverdi_TEX_OUTPUTS ${Monteverdi_TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.ind)
#   ENDIF(MAKEINDEX_COMPILER)
# 
#   IF(BIBTEX_COMPILER)
#     ADD_CUSTOM_COMMAND(
#       SOURCE    MonteverdiGuide.aux
#       COMMAND   ${BIBTEX_COMPILER}
#       ARGS      MonteverdiGuide
#       OUTPUT    ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.bbl
#       COMMENT   "Generating Monteverdi bbl file"
#     )
#   SET(Monteverdi_TEX_OUTPUTS ${Monteverdi_TEX_OUTPUTS} ${SoftwareGuide_SOURCE_DIR}/Latex/MonteverdiGuide.bbl)
#   ENDIF(BIBTEX_COMPILER)
  
  ADD_CUSTOM_TARGET(MonteverdiGuideLatex ALL DEPENDS ${Monteverdi_TEX_OUTPUTS})
  ENDIF (GENERATE_MONTEVERDI_DOC)
           
ENDIF(LATEX_COMPILER)


ADD_CUSTOM_TARGET(SoftwareGuideLatex ALL DEPENDS ${TEX_OUTPUTS})

