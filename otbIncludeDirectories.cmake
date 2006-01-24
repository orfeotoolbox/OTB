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
  ${OTB_SOURCE_DIR}/Code/IO
  ${OTB_SOURCE_DIR}/Code/ChangeDetection
  ${OTB_SOURCE_DIR}/Code/FeatureExtraction
  ${OTB_SOURCE_DIR}/Code/Learning
  ${OTB_SOURCE_DIR}/Code/Visu
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
  ${OTB_INSTALL_INCLUDE_DIR}/IO
  ${OTB_INSTALL_INCLUDE_DIR}/ChangeDetection
  ${OTB_INSTALL_INCLUDE_DIR}/FeatureExtraction
  ${OTB_INSTALL_INCLUDE_DIR}/Learning
  ${OTB_INSTALL_INCLUDE_DIR}/Visu
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
