# these are cache variables, so they could be overwritten with -D,
set(CPACK_PACKAGE_NAME "OTB"
    CACHE STRING "The OTB full package"
)
# which is useful in case of packing only selected components instead of the whole thing
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OTB Package"
    CACHE STRING "This package contains the entire OTB project by modules OR in one full package"
)
set(CPACK_PACKAGE_VENDOR "CS Group")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/build_packages")

set(CPACK_PACKAGE_VERSION_MAJOR ${OTB_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTB_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTB_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT "thibaut.romain@csgroup.eu")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Thibaut ROMAIN")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
# package name for deb. If set, then instead of some-application-0.9.2-Linux.deb
# you'll get some-application_0.9.2_amd64.deb (note the underscores too)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
# that is if you want every group to have its own package,
# although the same will happen if this is not set (so it defaults to ONE_PER_GROUP)
# and CPACK_DEB_COMPONENT_INSTALL is set to YES
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
# without this you won't be able to pack only specified component
set(CPACK_COMPONENTS_ALL Core FeaturesExtraction Hyperspectral Miscellaneous Learning Segmentation Sar StereoProcessing Miscellaneous Dependencies)
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)

cpack_add_component_group(Core
                         [DISPLAY_NAME Core]
                         [DESCRIPTION "Main Group for Core of OTB" ]
                         [EXPANDED]
                         [BOLD_TITLE]
                         [REQUIRED])

cpack_add_component(FeaturesExtraction
                    [DISPLAY_NAME FeaturesExtraction]
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
