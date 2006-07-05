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
  ${OTB_SOURCE_DIR}/Code/Visu
)

#-----------------------------------------------------------------------------
# Include directories from the GDAL build tree.
# IF(NOT OTB_USE_EXTERNAL_GDAL)
#  	INCLUDE(${OTB_SOURCE_DIR}/Utilities/GDAL/gdalIncludeDirectories.cmake)
#  	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
#    		${GDAL_INCLUDE_DIRS_BUILD_TREE}
#    	)
# ENDIF(NOT OTB_USE_EXTERNAL_GDAL)
#    		$ENV{GDAL_DIR_LIB}
#THOMAS


SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  		${GDAL_INCLUDE_DIRS} )


#-----------------------------------------------------------------------------
# Include directories from the ITK build tree.e

#THOMAS
IF(OTB_USE_EXTERNAL_ITK)
    INCLUDE(${ITK_SOURCE_DIR}/Utilities/itkThirdParty.cmake)
 	INCLUDE(${ITK_SOURCE_DIR}/itkIncludeDirectories.cmake)
ELSE(OTB_USE_EXTERNAL_ITK)
 	INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/itkIncludeDirectories.cmake)
ENDIF(OTB_USE_EXTERNAL_ITK)


#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in OTB.  These include
# directories will NOT be available to user projects.
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX)

SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX} )

#THOMAS
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${GDAL_INCLUDE_DIRS} )

#THOMAS
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
SET(OTB_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/otb")
SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  ${OTB_INSTALL_INCLUDE_DIR}
  ${OTB_INSTALL_INCLUDE_DIR}/Common
  ${OTB_INSTALL_INCLUDE_DIR}/BasicFilters
  ${OTB_INSTALL_INCLUDE_DIR}/IO
  ${OTB_INSTALL_INCLUDE_DIR}/ChangeDetection
  ${OTB_INSTALL_INCLUDE_DIR}/FeatureExtraction
  ${OTB_INSTALL_INCLUDE_DIR}/Learning
  ${OTB_INSTALL_INCLUDE_DIR}/Visu
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK
#THOMAS
#  ${ITK_INSTALL_INCLUDE_DIR}
#  ${ITK_INCLUDE_DIRS}
)

#THOMAS
SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		${GDAL_INCLUDE_DIRS} )

#THOMAS
#THOMAS
IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
   			 ${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
			${ITK_INCLUDE_DIRS_BUILD_TREE} 
			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} )
ENDIF(OTB_USE_EXTERNAL_ITK)

#SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE} 
#		${ITK_INCLUDE_DIRS_INSTALL_TREE}  )
#  		${ITK_INCLUDE_DIRS} )



#IF(NOT OTB_USE_SYSTEM_VXL)
#  SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
#    ${OTB_INSTALL_INCLUDE_DIR}/Utilities/vxl/vcl
#    ${OTB_INSTALL_INCLUDE_DIR}/Utilities/vxl/core
#    )
#ENDIF(NOT OTB_USE_SYSTEM_VXL)

#-----------------------------------------------------------------------------
# Include directories for 3rd-party utilities provided by OTB.
#OTB_THIRD_PARTY_INCLUDE(ZLIB zlib)
#OTB_THIRD_PARTY_INCLUDE(PNG  png)
#OTB_THIRD_PARTY_INCLUDE(TIFF tiff)
