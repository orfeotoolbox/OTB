# Sources of non-templated classes.


FILE(GLOB OTBLearning_SRCS "*.cxx" )

ADD_LIBRARY(OTBLearning ${OTBLearning_SRCS})
TARGET_LINK_LIBRARIES (OTBLearning otbsvm OTBCommon OTBIO ITKCommon ITKIO)

INSTALL_TARGETS(/lib/otb OTBLearning )
INSTALL_FILES(/include/otb/Learning "(\\.h|\\.txx)$")
