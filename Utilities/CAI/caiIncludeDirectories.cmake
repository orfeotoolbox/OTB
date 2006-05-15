# $Id$


#-----------------------------------------------------------------------------
# Include directories from the build tree.
IF(WIN32)
        SET(CAI_INCLUDE_DIRS_BUILD_TREE 
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll/inc
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll/inc/inc_unix2win
        )
ELSE(WIN32)
        SET(CAI_INCLUDE_DIRS_BUILD_TREE
                ${OTB_SOURCE_DIR}/Utilities/CAI
                ${OTB_SOURCE_DIR}/Utilities/CAI/inc
        )
ENDIF(WIN32)

