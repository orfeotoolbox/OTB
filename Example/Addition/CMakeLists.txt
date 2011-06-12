
SET( SRCS
     otbAddition.cxx )

ADD_LIBRARY(OTBWrapperAddition otbAddition.cxx)
TARGET_LINK_LIBRARIES(OTBWrapperAddition OTBWrapperCore)
