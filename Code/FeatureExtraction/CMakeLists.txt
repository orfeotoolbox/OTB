# Sources of non-templated classes.


FILE(GLOB OTBFeatureExtraction_SRCS "*.cxx" )

ADD_LIBRARY(OTBFeatureExtraction ${OTBFeatureExtraction_SRCS})
TARGET_LINK_LIBRARIES (OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO ITKAlgorithms)

INSTALL_TARGETS(/lib/otb OTBFeatureExtraction )
INSTALL_FILES(/include/otb/FeatureExtraction "(\\.h|\\.txx)$")
