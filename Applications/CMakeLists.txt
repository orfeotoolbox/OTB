include(OTBWrapperMacros)

# This var will contain the list of all application name
# usefull for batch testing
set(OTB_APPLICATIONS_NAME_LIST "" CACHE STRING "List of all applications name" FORCE)

add_subdirectory(ChangeDetection)
add_subdirectory(Classification)
add_subdirectory(FeatureExtraction)
add_subdirectory(Hyperspectral)
add_subdirectory(Projections)
add_subdirectory(Segmentation)
add_subdirectory(Util)
add_subdirectory(Radiometry)

add_subdirectory(Test)
