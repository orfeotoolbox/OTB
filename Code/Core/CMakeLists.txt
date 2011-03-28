FILE(GLOB srcs "*.cxx")

ADD_LIBRARY(OTBWrapperCore ${srcs})
TARGET_LINK_LIBRARIES(OTBWrapperCore OTBCommon OTBIO)
