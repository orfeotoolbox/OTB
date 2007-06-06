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
  ${OTB_SOURCE_DIR}/Utilities/BGL
  ${OTB_SOURCE_DIR}/Utilities/BGL/boost
  ${OTB_SOURCE_DIR}/Utilities/otbsvm
  ${OTB_SOURCE_DIR}/Utilities/OSSIM
  ${OTB_SOURCE_DIR}/Utilities/OSSIM/include
  ${OTB_SOURCE_DIR}/Utilities/OSSIM/include/ossim
  ${OTB_SOURCE_DIR}/Utilities/dxflib
  ${OTB_SOURCE_DIR}/Utilities/InsightJournal
)

#-----------------------------------------------------------------------------
# Include directories from the ossim build tree
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
${OTB_BINARY_DIR}/Utilities/OSSIM/include
)


#-----------------------------------------------------------------------------
# Include directories from the GDAL build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  		${GDAL_INCLUDE_DIRS} )

#-----------------------------------------------------------------------------
# Include directories from the ITK build tree.e
IF(OTB_USE_EXTERNAL_ITK)
        INCLUDE(${ITK_SOURCE_DIR}/Utilities/itkThirdParty.cmake)
 	INCLUDE(${ITK_SOURCE_DIR}/itkIncludeDirectories.cmake)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib
 	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib/opt)
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

SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${GDAL_INCLUDE_DIRS} ${FLTK_INCLUDE_DIRS} )

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
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/BGL
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/BGL/boost
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsvm
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/InsightJournal
)

SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		${GDAL_INCLUDE_DIRS} ${FLTK_INCLUDE_DIRS})

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
