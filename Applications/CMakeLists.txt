include(OTBWrapperMacros)

# This var will contain the list of all application name
# usefull for batch testing
set(OTB_APPLICATIONS_NAME_LIST "" CACHE STRING "List of all applications name" FORCE)
mark_as_advanced(OTB_APPLICATIONS_NAME_LIST)

add_subdirectory(CartographicDBValidation)
add_subdirectory(ChangeDetection)
add_subdirectory(Classification)
add_subdirectory(DisparityMap)
add_subdirectory(FeatureExtraction)
add_subdirectory(Fusion)
add_subdirectory(Hyperspectral)
add_subdirectory(Projections)
add_subdirectory(Radiometry)
add_subdirectory(Rasterization)
add_subdirectory(Segmentation)
add_subdirectory(Test)
add_subdirectory(Utils)
add_subdirectory(ContrastEnhancement)