#####################################################
#
#   This is the root SoftwareGuide CMakeLists file.
#
#####################################################
#
# To Build the OTB Software Guide,
# 
# Compile and build the OTB binaries. You will need to build the Examples in 
# the Insight directory with the CMakeLists.txt file in Insight/
#
# Run this CMakeLists.txt file and set BUILD_FIGURES  'ON'. Specify the 
# directory containing the Insight executables. Specify OTB data paths.
# The default data path inferred is Insight/Examples/Data. This should suffice
# for building the SoftwareGuide. Should more paths be need add them in a colon
# seperated list. 
#
# Make
# Make
# 
# More than one make may be necessary to ensure generation of the index
# 
# If BUILD_FIGURES is set to ON, the examples source files are parsed for 
# command line argument tags and if found executed to generate output 
# images, if not the images are expected to be present in the Art/ folder
# in PNG/XFIG/JPEG/EPS format. 
# 
# Since generating figures may take a while, you may set BUILD_FIGURES to 
# OFF. In this case, all figures are expected to be present in the Art/
# folder in PNG/XFIG/JPEG/EPS format. 
#  
#
CMAKE_MINIMUM_REQUIRED(VERSION 1.6)

PROJECT(SoftwareGuide C)



#
# File that defines the path to .cxx examples in OTB sources
#
CONFIGURE_FILE(
${SoftwareGuide_SOURCE_DIR}/SoftwareGuideConfiguration.tex.in 
${SoftwareGuide_BINARY_DIR}/SoftwareGuideConfiguration.tex
IMMEDIATE
)

# Find OTB Sources
#FIND_PATH(OTB_SOURCE_DIR UseOTB.cmake.in
#   /ORFEO/thomas/ORFEO-TOOLBOX/otb/OTB
#  )

FIND_PACKAGE(OTB)
IF(OTB_FOUND)
  INCLUDE(${OTB_USE_FILE})
ELSE(OTB_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build OTB-Documents without OTB.  Please set OTB_DIR.")
ENDIF(OTB_FOUND)


IF( NOT OTB_SOURCE_DIR )
  MESSAGE(FATAL_ERROR "OTB source directory is not set")
ENDIF( NOT OTB_SOURCE_DIR )

# OTB_DATA_PATHS is searched recursively.. you need not enter sub-directories
SET(OTB_DATA_PATHS "${OTB_SOURCE_DIR}/Examples/Data" CACHE STRING "Where the OTB data is. Enter a colon seperated list.")

#
# Rebuild the Software Guide figures or not ?
SET(BUILD_FIGURES ON CACHE BOOL "Build Software Guide figures")

IF( BUILD_FIGURES )
  SET(OTB_EXECUTABLES_DIR "${OTB_LIBRARY_DIRS}" CACHE PATH "Where the OTB examples executables are")
  SET(RUN_EXAMPLES_SCRIPT "${SoftwareGuide_SOURCE_DIR}/Examples/RunExamples.pl" CACHE FILEPATH "Where the RunExamples Script is")
ENDIF( BUILD_FIGURES )


# 
# Find ImageMagick tools
# This is used to convert image formats
#
INCLUDE (${CMAKE_ROOT}/Modules/FindImageMagick.cmake)
IF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)
  MESSAGE("ImageMagick convert utility was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT IMAGEMAGICK_CONVERT_EXECUTABLE)


IF (BUILD_FIGURES)

  # 
  #
  # ADD THE LIST OF INPUT IMAGES YOU WANT FLIPPED HERE. 
  # 
  # NOTE:
  # If ON, all input images in the list and all outputs that use these images
  # as inputs in cmd line args will be flipped about 'y'. The command line
  # argument itself does not run using the flipped images; instead the eps
  # images corresponding to the inputs mentioned in the list and their output
  # dependencies are flipped for inclusion in the TEX document.

  OPTION(OTB_FLIP_INPUTS_AND_THEIR_OUTPUTS "Flip the input images specified in CMakeLists and their corresponding outputs" ON)
  SET(OTB_FLIP_IMG
    BinaryImage
  )
  # END FLIP_INPUTS LIST


  SUBDIRS(Examples)
  SUBDIRS(Art)
  SUBDIRS(Latex)

ELSE( BUILD_FIGURES )
  SUBDIRS(Art)
  SUBDIRS(Examples)
  SUBDIRS(Latex)
ENDIF( BUILD_FIGURES )

# 
# Examples Directory is where .cxx files are converted 
# into .tex by a perl script.
#
# Art Directory with images and diagrams
#
# Latex Directory with tex files 
#
