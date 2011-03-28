FILE(GLOB srcs "*.cxx")

ADD_LIBRARY(otbWrapperCore ${srcs})
TARGET_LINK_LIBRARIES(otbWrapperCore OTBCommon OTBIO)
