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
# CAI directories
IF(WIN32)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll/inc
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll/inc/inc_unix2win
        )
ELSE(WIN32)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
                ${OTB_SOURCE_DIR}/Utilities/CAI
                ${OTB_SOURCE_DIR}/Utilities/CAI/inc
        )
ENDIF(WIN32)

SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/raw
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff/libgeotiff
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff/libtiff
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff
#  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/ceos
#  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/envisat
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/msg
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/msg/PublicDecompWT/COMP/WT/Inc
  ${OTB_SOURCE_DIR}/Utilities/GDAL/gcore
  ${OTB_SOURCE_DIR}/Utilities/GDAL/ogr
  ${OTB_SOURCE_DIR}/Utilities/GDAL/ogr/ogrsf_frmts
  ${OTB_SOURCE_DIR}/Utilities/GDAL/port
  ${OTB_SOURCE_DIR}/Utilities/GDAL/alg
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
