ADD_LIBRARY(ITKBasicFilters itkVTKImageExportBase.cxx)
TARGET_LINK_LIBRARIES(ITKBasicFilters ITKCommon)
INSTALL_TARGETS(/lib/otb ITKBasicFilters)
INSTALL_FILES(/include/otb/Utilities/ITK/BasicFilters "(\\.h|\\.txx)$")
