# Sources for ITKStatistics library
ADD_LIBRARY(ITKStatistics 
  itkNormalVariateGenerator.cxx
  itkDenseFrequencyContainer.cxx
  itkSparseFrequencyContainer.cxx
 )

TARGET_LINK_LIBRARIES(ITKStatistics ITKCommon)

INSTALL_TARGETS(/lib/otb ITKStatistics)
INSTALL_FILES(/include/otb/Utilities/ITK/Numerics/Statistics "(\\.h|\\.txx)$")
