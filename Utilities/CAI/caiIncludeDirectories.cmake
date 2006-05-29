# $Id$

#-----------------------------------------------------------------------------
# Include directories from the build tree.
IF(UNIX OR CYGWIN OR MINGW)
        SET(CAI_INCLUDE_DIRS_BUILD_TREE
                ${OTB_SOURCE_DIR}/Utilities/CAI
                ${OTB_SOURCE_DIR}/Utilities/CAI/inc
        )
ELSE(UNIX OR CYGWIN OR MINGW)
        SET(CAI_INCLUDE_DIRS_BUILD_TREE 
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll/inc
                ${OTB_SOURCE_DIR}/Utilities/CAI/cai_dll/inc/inc_unix2win
        )
ENDIF(UNIX OR CYGWIN OR MINGW)
