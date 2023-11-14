# these are cache variables, so they could be overwritten with -D,
set(CPACK_PACKAGE_NAME "OTB"
    CACHE STRING "The OTB full package"
)
# which is useful in case of packing only selected components instead of the whole thing
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Light OTB Package"
    CACHE STRING "This package contains only a part of OTB"
)
set(CPACK_PACKAGE_VENDOR "CS Group")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/build_packages")

# https://unix.stackexchange.com/a/11552/254512
#set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/otb")#/${CMAKE_PROJECT_VERSION}")

set(CPACK_PACKAGE_VERSION_MAJOR ${OTB_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTB_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTB_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT "thibaut.romain@csgroup.eu")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Thibaut ROMAIN")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# package name for deb. If set, then instead of some-application-0.9.2-Linux.deb
# you'll get some-application_0.9.2_amd64.deb (note the underscores too)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
# that is if you want every group to have its own package,
# although the same will happen if this is not set (so it defaults to ONE_PER_GROUP)
# and CPACK_DEB_COMPONENT_INSTALL is set to YES
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
# without this you won't be able to pack only specified component
set(CPACK_DEB_COMPONENT_INSTALL ON)
if(UNIX)
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
else()
    set(CPACK_ARCHIVE_COMPONENT_INSTALL OFF)
endif()
set(CPACK_COMPONENTS_ALL Core ImageProcessing Hyperspectral Miscellaneous Learning Segmentation Sar StereoProcessing Remote Dependencies)
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Packaging)
include(patch_cmake_files)
include(install_without_message)

patch_cmake_files(NAME "ITK"
VERSION "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}"
MATCH_STRING  "${XDK_INSTALL_PATH}"
REPLACE_VAR "ITK_INSTALL_PREFIX"
)

patch_cmake_files(NAME "OTB"
VERSION "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}"
MATCH_STRING  "${XDK_INSTALL_PATH}"
REPLACE_VAR "OTB_INSTALL_PREFIX"
)

include(CPack)

cpack_add_component_group(Core
                         [DISPLAY_NAME Core]
                         [DESCRIPTION "Main Group for Core of OTB" ]
                         [EXPANDED]
                         [BOLD_TITLE]
                         [REQUIRED])

cpack_add_component(ImageProcessing
                    [DISPLAY_NAME ImageProcessing]
                    [DESCRIPTION "Contains all Image processing libraries and applications for OTB" ]
                    [OPTIONAL])
                 
cpack_add_component(Learning
                    [DISPLAY_NAME Machine_Learning]
                    [DESCRIPTION "Contains all Machine learning libraries and applications for OTB" ]
                    [OPTIONAL])
                    
cpack_add_component(Hyperspectral
                    [DISPLAY_NAME Hyperspectral]
                    [DESCRIPTION "Contains all Hyperspectral libraries and applications for OTB" ]
                    [OPTIONAL])

cpack_add_component(Miscellaneous
                    [DISPLAY_NAME Miscellaneous]
                    [DESCRIPTION "Contains Miscellaneous libraries and applications for OTB" ]
                    [OPTIONAL])

cpack_add_component(Segmentation
                    [DISPLAY_NAME Segmentation]
                    [DESCRIPTION "Contains all Segmentation libraries and applications for OTB" ]
                    [OPTIONAL])

cpack_add_component(Sar
                    [DISPLAY_NAME Sar]
                    [DESCRIPTION "Contains all SAR libraries and applications for OTB" ]
                    [OPTIONAL])

cpack_add_component(Remote
                    [DISPLAY_NAME Remote]
                    [DESCRIPTION "Contains all Remote modules libraries and applications for OTB" ]
                    [OPTIONAL])

cpack_add_component(StereoProcessing
                    [DISPLAY_NAME Stereo]
                    [DESCRIPTION "Contains all Stereo libraries and applications for OTB" ]
                    [OPTIONAL])