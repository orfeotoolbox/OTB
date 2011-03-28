
FILE(GLOB srcs "*.cxx")

ADD_LIBRARY(OTBWrapperQtWidget ${srcs})
TARGET_LINK_LIBRARIES(OTBWrapperQtWidget OTBCommon OTBIO OTBWrapperCore ${QT_LIBRARIES})
