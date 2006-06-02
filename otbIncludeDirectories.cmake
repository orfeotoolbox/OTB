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
#IF(NOT OTB_USE_EXTERNAL_GDAL)
# 	INCLUDE(${OTB_SOURCE_DIR}/Utilities/GDAL/gdalIncludeDirectories.cmake)
# 	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
#   		${GDAL_INCLUDE_DIRS_BUILD_TREE}
#   	)
#ENDIF(NOT OTB_USE_EXTERNAL_GDAL)

#-----------------------------------------------------------------------------
# Include directories from the ITK build tree.e
IF(NOT OTB_USE_EXTERNAL_ITK)
 	INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/itkIncludeDirectories.cmake)
 	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
   		${ITK_INCLUDE_DIRS_BUILD_TREE}
   	)
ENDIF(NOT OTB_USE_EXTERNAL_ITK)

#-----------------------------------------------------------------------------
# Include directories from the FLTK build tree.
#IF(NOT OTB_USE_EXTERNAL_FLTK)
# 	INCLUDE(${OTB_SOURCE_DIR}/Utilities/FLTK/fltkIncludeDirectories.cmake)
# 	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
#   		${FLTK_INCLUDE_DIRS_BUILD_TREE}
#   	)
#ENDIF(NOT OTB_USE_EXTERNAL_FLTK)


#-----------------------------------------------------------------------------
# Include directories from the CAI build tree.
INCLUDE(${OTB_SOURCE_DIR}/Utilities/CAI/caiIncludeDirectories.cmake)
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${CAI_INCLUDE_DIRS_BUILD_TREE}
  )




#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in OTB.  These include
# directories will NOT be available to user projects.
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX)

SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX} )

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
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/CAI
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/GDAL
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK
)


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
