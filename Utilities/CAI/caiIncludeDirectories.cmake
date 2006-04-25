# $Id$
#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(CAI_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(CAI_INCLUDE_DIRS_BUILD_TREE ${CAI_BINARY_DIR})

# These directories are always needed.
SET(CAI_INCLUDE_DIRS_BUILD_TREE ${CAI_INCLUDE_DIRS_BUILD_TREE}
  ${CAI_SOURCE_DIR}/inc
)

#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in CAI.  These include
# directories will NOT be available to user projects.
SET(CAI_INCLUDE_DIRS_BUILD_TREE_CXX)

SET(CAI_INCLUDE_DIRS_BUILD_TREE_CXX ${CAI_INCLUDE_DIRS_BUILD_TREE_CXX} )

#-----------------------------------------------------------------------------
# Include directories from the install tree.
SET(CAI_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/cai")
SET(CAI_INCLUDE_DIRS_INSTALL_TREE ${CAI_INCLUDE_DIRS_INSTALL_TREE}
  ${CAI_INSTALL_INCLUDE_DIR}
)

