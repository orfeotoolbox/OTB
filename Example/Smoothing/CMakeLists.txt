
SET( SRCS
     otbSmoothing.cxx )

ADD_LIBRARY(OTBWrapperSmoothing otbSmoothing.cxx)
TARGET_LINK_LIBRARIES(OTBWrapperSmoothing OTBWrapperCore)
