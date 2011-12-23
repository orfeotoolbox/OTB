# $Id$
#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(OTB_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_BINARY_DIR})


# Include directories from the Boost build tree.
IF(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${Boost_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Utilities/BGL
)
ENDIF(OTB_USE_EXTERNAL_BOOST)

#-----------------------------------------------------------------------------
# Include directories from the GDAL build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${GDAL_INCLUDE_DIR} )

IF(WIN32)
  SET(OTB_MSINTTYPES_INCLUDE_DIR_BUILDTREE ${OTB_SOURCE_DIR}/Utilities/otbmsinttypes )
ENDIF(WIN32)

# These directories are always needed.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_SOURCE_DIR}/Code/BasicFilters
  ${OTB_SOURCE_DIR}/Code/ChangeDetection
  ${OTB_SOURCE_DIR}/Code/Common
  ${OTB_SOURCE_DIR}/Code/DisparityMap
  ${OTB_SOURCE_DIR}/Code/FeatureExtraction
  ${OTB_SOURCE_DIR}/Code/Fusion
  ${OTB_SOURCE_DIR}/Code/Fuzzy
  ${OTB_SOURCE_DIR}/Code/GeospatialAnalysis
  ${OTB_SOURCE_DIR}/Code/Gui
  ${OTB_SOURCE_DIR}/Code/Hyperspectral
  ${OTB_SOURCE_DIR}/Code/IO
  ${OTB_SOURCE_DIR}/Code/Learning
  ${OTB_SOURCE_DIR}/Code/Markov
  ${OTB_SOURCE_DIR}/Code/MultiScale
  ${OTB_SOURCE_DIR}/Code/MultiTemporal
  ${OTB_SOURCE_DIR}/Code/OBIA
  ${OTB_SOURCE_DIR}/Code/ObjectDetection
  ${OTB_SOURCE_DIR}/Code/Projections
  ${OTB_SOURCE_DIR}/Code/Radiometry
  ${OTB_SOURCE_DIR}/Code/SARPolarimetry
  ${OTB_SOURCE_DIR}/Code/Simulation
  ${OTB_SOURCE_DIR}/Code/SpatialReasoning
  ${OTB_SOURCE_DIR}/Code/Testing
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/CurlAdapters
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/OssimAdapters
  ${OTB_SOURCE_DIR}/Code/Visu
  ${OTB_SOURCE_DIR}/Code/Visualization
  ${OTB_BINARY_DIR}/Code/Visualization
  ${OTB_SOURCE_DIR}/Code/ApplicationEngine
  ${OTB_MSINTTYPES_INCLUDE_DIR_BUILDTREE}
  ${OTB_SOURCE_DIR}/Utilities/otbsvm
  ${OTB_SOURCE_DIR}/Utilities/otbossimplugins
  ${OTB_SOURCE_DIR}/Utilities/otbossimplugins/ossim
  ${OTB_SOURCE_DIR}/Utilities/InsightJournal
  ${OTB_SOURCE_DIR}/Utilities/otb6S
  ${OTB_SOURCE_DIR}/Utilities/tinyXMLlib
#  ${OTB_SOURCE_DIR}/Utilities/otbgalib
  ${OTB_SOURCE_DIR}/Utilities/otbkml/src
  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party
  ${OTB_SOURCE_DIR}/Utilities/otbliblas/include
  ${OTB_SOURCE_DIR}/Utilities/otbedison/segm
  ${OTB_SOURCE_DIR}/Utilities/otbedison/prompt
  ${OTB_SOURCE_DIR}/Utilities/otbedison/edge
  ${OTB_SOURCE_DIR}/Utilities/otbsiftfast
  ${OTB_SOURCE_DIR}/Utilities/otbconfigfile
  ${OTB_SOURCE_DIR}/Utilities/otbknn
  ${OTB_SOURCE_DIR}/Utilities/otbmuparser
)

IF(BUILD_APPLICATIONS)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Code/Wrappers/CommandLine )
ENDIF(BUILD_APPLICATIONS)

IF(NOT OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party/boost_1_34_1)
ELSE(NOT OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${Boost_INCLUDE_DIR})
ENDIF(NOT OTB_USE_EXTERNAL_BOOST)

IF(NOT OTB_USE_EXTERNAL_OSSIM)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim
  ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim)
ELSE(NOT OTB_USE_EXTERNAL_OSSIM)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OSSIM_INCLUDE_DIR})
ENDIF(NOT OTB_USE_EXTERNAL_OSSIM)

IF(OTB_USE_PQXX)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Code/GeospatialAnalysis)
ENDIF(OTB_USE_PQXX)


#-----------------------------------------------------------------------------
# Include directories from the ossim build tree
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
        ${OTB_BINARY_DIR}/Utilities/otbossim/include
)



#-----------------------------------------------------------------------------
# Include directories from the CURL build tree.
IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

#-----------------------------------------------------------------------------
# Include directories from the MAPNIK build tree.
IF(OTB_USE_MAPNIK)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${MAPNIK_INCLUDE_DIR} ${FREETYPE2_INCLUDE_DIR})
ENDIF(OTB_USE_MAPNIK)

#-----------------------------------------------------------------------------
# Include directories from the GETTEXT build tree.
IF(OTB_USE_GETTEXT)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${GETTEXT_INCLUDE_DIR})
ENDIF(OTB_USE_GETTEXT)

#-----------------------------------------------------------------------------
# Include directories from the PQXX build tree.
IF(OTB_USE_PQXX)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${PQXX_INCLUDE_DIR})
ENDIF(OTB_USE_PQXX)


#-----------------------------------------------------------------------------
# Include directories from the ITK build tree.
IF(OTB_USE_EXTERNAL_ITK)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${ITK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_ITK)
	IF(NOT ITK_INSTALL_INCLUDE_DIR)
  		SET(ITK_INSTALL_INCLUDE_DIR "/include/otb/Utilities/ITK")
	ENDIF(NOT ITK_INSTALL_INCLUDE_DIR)
#         INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkThirdParty.cmake)
#  	INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/itkIncludeDirectories.cmake)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Algorithms
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/BasicFilters
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Common
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/IO
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Numerics
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Numerics/FEM
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Numerics/NeuralNetworks
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Numerics/Statistics
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Patented
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/Review
        ${OTB_SOURCE_DIR}/Utilities/ITK/Code/SpatialObject
        ${OTB_BINARY_DIR}/Utilities/ITK
        ${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/v3p/netlib
        ${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/vcl
        ${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/core
        ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/vxl/vcl
        ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/vxl/core
        ${OTB_BINARY_DIR}/Utilities/ITK/Utilities
        ${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkExtHdrs
        )
ENDIF(OTB_USE_EXTERNAL_ITK)

#For  header file
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      		${FLTK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
                ${FLTK_SOURCE_DIR}
            	${FLTK_SOURCE_DIR}/src
            	${FLTK_BINARY_DIR} )
ENDIF(OTB_USE_EXTERNAL_FLTK)

#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in OTB.  These include
# directories will NOT be available to user projects.
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX)

SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX} )

#-----------------------------------------------------------------------------
# Include FLTK variables.
#IF(NOT OTB_USE_EXTERNAL_FLTK)
#        IF(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#                MESSAGE(FATAL_ERROR "Impossible to find the file ${FLTK_BINARY_DIR}/FLTKConfig.cmake. You must generate FLTK with cmake process.")
#        ELSE(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#                INCLUDE(${FLTK_BINARY_DIR}/FLTKConfig.cmake)
#        ENDIF(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#ELSE(OTB_USE_EXTERNAL_FLTK)
#        IF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
#                INCLUDE(${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake)
#        ENDIF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
#ENDIF(NOT OTB_USE_EXTERNAL_FLTK)



#-----------------------------------------------------------------------------
# Include directories from libraries build tree.


#For GLU header file
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${OTB_GLU_INCLUDE_PATH} )

#For EXPAT header file
IF(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  	        ${EXPAT_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbexpat
                ${OTB_SOURCE_DIR}/Utilities/otbexpat/lib)
ENDIF(OTB_USE_EXTERNAL_EXPAT)

# For OpenThreads
IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbopenthreads/OpenThreads/include
	)
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

#For ITK header file
IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS_BUILD_TREE}
   			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} 	)
ENDIF(OTB_USE_EXTERNAL_ITK)







#-----------------------------------------------------------------------------
# Include directories from the install tree.
# Alls install dirs must been RELATIVE PATH, except for the external libraries, specify by the ABSOLUE PATH
SET(OTB_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}${OTB_INSTALL_INCLUDE_DIR}")
#           SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS} )

#           SET(OTB_INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/otb")
#           SET(OTB_INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
#           SET(OTB_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/otb")

SET(OTB_INCLUDE_RELATIVE_DIRS )
SET(OTB_INCLUDE_ABSOLUE_DIRS )

#For BOOST header file
IF(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${Boost_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
        Utilities/BGL)
ENDIF(OTB_USE_EXTERNAL_BOOST)


IF(WIN32)
  SET(OTB_MSINTTYPES_INCLUDE_RELATIVE_DIR Utilities/otbmsinttypes )
ENDIF(WIN32)


SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
  Common
  BasicFilters
  IO
  ChangeDetection
  FeatureExtraction
  Learning
  MultiScale
  MultiTemporal
  SpatialReasoning
  DisparityMap
  Visu
  Visualization
  Gui
  Projections
  Radiometry
  SARPolarimetry
  Markov
  Fusion
  GeospatialAnalysis
  Hyperspectral
  Simulation
  Testing
  UtilitiesAdapters/CurlAdapters
  UtilitiesAdapters/OssimAdapters
  OBIA
  Fuzzy
  ObjectDetection
  ApplicationEngine
  Utilities
  Utilities/ITK
  ${OTB_MSINTTYPES_INCLUDE_RELATIVE_DIR}
  Utilities/otbsvm
  Utilities/InsightJournal
  Utilities/otb6S
  Utilities/tinyXMLlib
#  Utilities/otbgalib
  Utilities/otbkml
  Utilities/otbkml/src
  Utilities/otbkml/third_party
  Utilities/otbkml/third_party/boost_1_34_1
  Utilities/otbliblas/include
  Utilities/otbedison
  Utilities/otbsiftfast
  Utilities/otbconfigfile
  Utilities/otbknn
  Utilities/otbmuparser
)

# For CommandLine wrappers
IF(BUILD_APPLICATIONS)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
    Wrappers/CommandLine )
ENDIF(BUILD_APPLICATIONS)

#For GDAL header file
SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  		${GDAL_INCLUDE_DIR} )

#For EXPAT header file
IF(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	        ${EXPAT_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
                Utilities/otbexpat)
ENDIF(OTB_USE_EXTERNAL_EXPAT)

#For CURL header file
IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

#For MAPNIK header file
IF(OTB_USE_MAPNIK)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${MAPNIK_INCLUDE_DIR} )
ENDIF(OTB_USE_MAPNIK)

#For Gettext header file
IF(OTB_USE_GETTEXT)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${GETTEXT_INCLUDE_DIR} )
ENDIF(OTB_USE_GETTEXT)

#For FLTK header file
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  		        ${FLTK_INCLUDE_DIRS})
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
  		        Utilities/FLTK)
ENDIF(OTB_USE_EXTERNAL_FLTK)

#For GLU header file
SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  		${OTB_GLU_INCLUDE_PATH} )

#For ITK header file
IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)

    #Add prefix path "Utilities/ITK" For ITK_INCLUDE_RELATIVE_DIRS paths
        SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
              Utilities/ITK/Algorithms
              Utilities/ITK/BasicFilters
              Utilities/ITK/Common
              Utilities/ITK/IO
              Utilities/ITK/Numerics
              Utilities/ITK/Numerics/FEM
              Utilities/ITK/Numerics/NeuralNetworks
              Utilities/ITK/Numerics/Statistics
              Utilities/ITK/Patented
              Utilities/ITK/Review
              Utilities/ITK/SpatialObject
              Utilities/ITK
              Utilities/ITK/Utilities/vxl/v3p/netlib
              Utilities/ITK/Utilities/vxl/vcl
              Utilities/ITK/Utilities/vxl/core
              Utilities/ITK/Utilities
              Utilities/ITK/Utilities/itkExtHdrs
)

ENDIF(OTB_USE_EXTERNAL_ITK)
