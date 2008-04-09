# $Id$
#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(OTB_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_BINARY_DIR})

# These directories are always needed.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_SOURCE_DIR}/Code/Common
  ${OTB_SOURCE_DIR}/Code/BasicFilters
  ${OTB_SOURCE_DIR}/Code/IO
  ${OTB_SOURCE_DIR}/Code/ChangeDetection
  ${OTB_SOURCE_DIR}/Code/FeatureExtraction
  ${OTB_SOURCE_DIR}/Code/Learning
  ${OTB_SOURCE_DIR}/Code/MultiScale
  ${OTB_SOURCE_DIR}/Code/SpatialReasoning
  ${OTB_SOURCE_DIR}/Code/DisparityMap
  ${OTB_SOURCE_DIR}/Code/Visu
  ${OTB_SOURCE_DIR}/Code/Gui
  ${OTB_SOURCE_DIR}/Code/Fusion
  ${OTB_SOURCE_DIR}/Code/Projections
  ${OTB_SOURCE_DIR}/Code/Radiometry
  ${OTB_SOURCE_DIR}/Code/Markov
  ${OTB_SOURCE_DIR}/Utilities/BGL
  ${OTB_SOURCE_DIR}/Utilities/BGL/boost
  ${OTB_SOURCE_DIR}/Utilities/otbsvm
  ${OTB_SOURCE_DIR}/Utilities/otbossim
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim
  ${OTB_SOURCE_DIR}/Utilities/dxflib
  ${OTB_SOURCE_DIR}/Utilities/InsightJournal
  ${OTB_SOURCE_DIR}/Utilities/otb6S
  ${OTB_SOURCE_DIR}/Utilities/otbgeotiff
  ${OTB_SOURCE_DIR}/Utilities/otbgeotiff/libxtiff
  ${OTB_SOURCE_DIR}/Utilities/tinyXMLlib
  ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg/libopenjpeg
)

#-----------------------------------------------------------------------------
# Include directories from the ossim build tree
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
        ${OTB_BINARY_DIR}/Utilities/otbossim/include
)


#-----------------------------------------------------------------------------
# Include directories from the GDAL build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${GDAL_INCLUDE_DIRS} )
#-----------------------------------------------------------------------------
# Include directories from the CURL build tree.
IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

#-----------------------------------------------------------------------------
# Include directories from the ITK build tree.
IF(OTB_USE_EXTERNAL_ITK)
#        INCLUDE(${ITK_SOURCE_DIR}/Utilities/itkThirdParty.cmake)
# 	INCLUDE(${ITK_SOURCE_DIR}/itkIncludeDirectories.cmake)
#	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
#	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib
# 	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib/opt)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
   			 ${ITK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_ITK)
	IF(NOT ITK_INSTALL_INCLUDE_DIR)
  		SET(ITK_INSTALL_INCLUDE_DIR "/include/otb/Utilities/ITK")
	ENDIF(NOT ITK_INSTALL_INCLUDE_DIR)
        INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkThirdParty.cmake)
 	INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/itkIncludeDirectories.cmake)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
	${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/v3p/netlib
	${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/v3p/netlib/opt)
ENDIF(OTB_USE_EXTERNAL_ITK)


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
# Include directories from the FLTK build tree.

IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${FLTK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/FLTK
  	        ${OTB_SOURCE_DIR}/Utilities/FLTK/src
  	        ${OTB_BINARY_DIR}/Utilities/FLTK )
ENDIF(OTB_USE_EXTERNAL_FLTK)

#For GLU header file
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${OTB_GLU_INCLUDE_PATH} )



SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${GDAL_INCLUDE_DIRS} )

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
SET(OTB_INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/otb")
SET(OTB_INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
SET(OTB_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/otb")
SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  ${OTB_INSTALL_INCLUDE_DIR}
  ${OTB_INSTALL_INCLUDE_DIR}/Common
  ${OTB_INSTALL_INCLUDE_DIR}/BasicFilters
  ${OTB_INSTALL_INCLUDE_DIR}/IO
  ${OTB_INSTALL_INCLUDE_DIR}/ChangeDetection
  ${OTB_INSTALL_INCLUDE_DIR}/FeatureExtraction
  ${OTB_INSTALL_INCLUDE_DIR}/Learning
  ${OTB_INSTALL_INCLUDE_DIR}/MultiScale
  ${OTB_INSTALL_INCLUDE_DIR}/SpatialReasoning
  ${OTB_INSTALL_INCLUDE_DIR}/DisparityMap
  ${OTB_INSTALL_INCLUDE_DIR}/Visu
  ${OTB_INSTALL_INCLUDE_DIR}/Gui
  ${OTB_INSTALL_INCLUDE_DIR}/Projections
  ${OTB_INSTALL_INCLUDE_DIR}/Radiometry
  ${OTB_INSTALL_INCLUDE_DIR}/Markov
  ${OTB_INSTALL_INCLUDE_DIR}/Fusion
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/BGL
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/BGL/boost
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsvm
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/InsightJournal
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otb6S
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbgeotiff
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbgeotiff/libxtiff
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/tinyXMLlib
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenjpeg/libopenjpeg
)

SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		${GDAL_INCLUDE_DIRS} )

IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		        ${FLTK_INCLUDE_DIRS})
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		        ${OTB_INSTALL_INCLUDE_DIR}/Utilities/FLTK)
ENDIF(OTB_USE_EXTERNAL_FLTK)
#For GLU header file
SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		${OTB_GLU_INCLUDE_PATH} )


IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)
#THOMAS
	SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
			${ITK_INCLUDE_DIRS_INSTALL_TREE} )
#			${ITK_INCLUDE_DIRS_BUILD_TREE} 
#			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} )
ENDIF(OTB_USE_EXTERNAL_ITK)
