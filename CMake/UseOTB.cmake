# This file sets up include directories, link directories, and
# compiler settings for a project to use OTB.  It should not be
# included directly, but rather through the OTB_USE_FILE setting
# obtained from OTBConfig.cmake.

# Add compiler flags needed to use OTB.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OTB_REQUIRED_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OTB_REQUIRED_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")


# Add include directories needed to use OTB.
include_directories(BEFORE ${OTB_INCLUDE_DIRS})

# Add link directories needed to use OTB.
link_directories(${OTB_LIBRARY_DIRS})

# TODO : find_package() & include(UseFile) for all external dependencies
