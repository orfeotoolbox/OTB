#
#  Convert images from different formats
#  into Encapsulated Postscript to be 
#  included in LaTeX files.
#


#
#  Find fig2dev : this program convert Xfig figures 
#  to Encapsulated Postscript on the command line.
#
FIND_PROGRAM(FIG2DEV_EXECUTABLE
  NAMES fig2dev
)


# 
# Find ImageMagick tools
# This is used to convert image formats
#
INCLUDE (${CMAKE_ROOT}/Modules/FindImageMagick.cmake)


#
#
#  Error checking for the components we need
#
#
IF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)
  MESSAGE("ImageMagick convert utility was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)

IF(OTB_USE_BMEPS)
  IF(NOT BMEPS_EXECUTABLE)
    MESSAGE("bmeps utility was not found. Please pass to advanced mode and provide its full path. It is part of DKTools available on http://dktools.sourceforge.net/")
  ENDIF(NOT BMEPS_EXECUTABLE)
ENDIF(OTB_USE_BMEPS)

ADD_CUSTOM_TARGET(SoftwareGuideArt  ALL echo)


#
#  At some point we should replace XFIG_FIGURES with the FILE GLOB expression.
#
FILE(GLOB XFIG_FIGURES *.fig)
FILE(GLOB PNG_IMAGES *.png)
FILE(GLOB JPEG_IMAGES *.jpg)
FILE(GLOB TIFF_IMAGES *.tif)
FILE(GLOB EPS_IMAGES *.eps)

FILE(GLOB MONTEVERDI_PNG_IMAGES ${CMAKE_CURRENT_SOURCE_DIR}/MonteverdiImages/*.png)
#
# If we built the figures, remove figures that were already generated 
# from the list
#
IF (BUILD_FIGURES)
  FIND_FILE(GENERATED_FILES "GeneratedFiles.txt" "${SoftwareGuide_BINARY_DIR}/Art/Generated")
  SET(GENERATED_FILES "${GENERATED_FILES}" CACHE INTERNAL "These files were generated" FORCE) #Just make the variable internal
  IF(GENERATED_FILES)
    FILE(READ ${GENERATED_FILES} REGULAR_EXPRESSIONS)
    FILE(GLOB FILES_TO_REMOVE ${REGULAR_EXPRESSIONS})
    LIST(LENGTH FILES_TO_REMOVE NUMBER_OF_FILES_TO_REMOVE)
    IF(NOT NUMBER_OF_FILES_TO_REMOVE MATCHES 0)
      MESSAGE("Cleaning up files: ${FILES_TO_REMOVE}")
      REMOVE(XFIG_FIGURES ${FILES_TO_REMOVE})
      REMOVE(PNG_IMAGES ${FILES_TO_REMOVE})
      REMOVE(JPEG_IMAGES ${FILES_TO_REMOVE})
      REMOVE(EPS_IMAGES ${FILES_TO_REMOVE})
      REMOVE(MONTEVERDI_PNG_IMAGES ${FILES_TO_REMOVE})
    ENDIF(NOT NUMBER_OF_FILES_TO_REMOVE MATCHES 0)
  ENDIF(GENERATED_FILES)
ENDIF(BUILD_FIGURES)





FOREACH(image ${TIFF_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  IF(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.tif
    COMMAND   ${BMEPS_EXECUTABLE}
    ARGS      "-ttiff" ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.tif
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ELSE(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.tif
    COMMAND   ${IMAGEMAGICK_CONVERT_EXECUTABLE}
    ARGS      ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.tif
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ENDIF(OTB_USE_BMEPS)
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${JPEG_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  IF(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.jpg
    COMMAND   ${BMEPS_EXECUTABLE}
    ARGS      "-tjpg" ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.jpg
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ELSE(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.jpg
    COMMAND   ${IMAGEMAGICK_CONVERT_EXECUTABLE}
    ARGS      ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.jpg
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ENDIF (OTB_USE_BMEPS)
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${PNG_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  IF(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.png
    COMMAND   ${BMEPS_EXECUTABLE}
    ARGS      "-tpng" ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.png
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ELSE(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.png
    COMMAND   ${IMAGEMAGICK_CONVERT_EXECUTABLE}
    ARGS      ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.png
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ENDIF(OTB_USE_BMEPS)
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${XFIG_FIGURES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  ADD_CUSTOM_COMMAND( 
  SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.fig
  COMMAND   ${FIG2DEV_EXECUTABLE}
  ARGS      -L eps ${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.fig ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
  TARGET    SoftwareGuideArt
  OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
  )
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${EPS_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  CONFIGURE_FILE(
                "${SoftwareGuide_SOURCE_DIR}/Art/${ImageBase}.eps" 
                "${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps" 
                COPYONLY IMMEDIATE)
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

FOREACH(image ${MONTEVERDI_PNG_IMAGES})
  GET_FILENAME_COMPONENT(Imagebase ${image} NAME)
  GET_FILENAME_COMPONENT(ImageBase ${Imagebase} NAME_WE)
  IF(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/MonteverdiImages/${ImageBase}.png
    COMMAND   ${BMEPS_EXECUTABLE}
    ARGS      ${SoftwareGuide_SOURCE_DIR}/Art/MonteverdiImages/${ImageBase}.png
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ELSE(OTB_USE_BMEPS)
    ADD_CUSTOM_COMMAND( 
    SOURCE    ${SoftwareGuide_SOURCE_DIR}/Art/MonteverdiImages/${ImageBase}.png
    COMMAND   ${IMAGEMAGICK_CONVERT_EXECUTABLE}
    ARGS      ${SoftwareGuide_SOURCE_DIR}/Art/MonteverdiImages/${ImageBase}.png
              ${ImageBase}.eps
    TARGET    SoftwareGuideArt
    OUTPUTS   ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps
    )
  ENDIF(OTB_USE_BMEPS)
  SET(EPS_DEPS ${EPS_DEPS} ${SoftwareGuide_BINARY_DIR}/Art/${ImageBase}.eps)
ENDFOREACH(image)

ADD_CUSTOM_COMMAND(
  SOURCE SoftwareGuideArt
  COMMAND echo
  TARGET SoftwareGuideArt
  DEPENDS ${EPS_DEPS}
)

