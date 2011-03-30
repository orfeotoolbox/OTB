
SET( SRCS
     otbAddition.cxx )

ADD_LIBRARY(OTBWrapperAddition otbAddition.cxx)
TARGET_LINK_LIBRARIES(OTBWrapperAddition OTBWrapperCore)

ADD_EXECUTABLE(otbWrapperAdditionExample-qt otbAdditionQt.cxx)
TARGET_LINK_LIBRARIES(otbWrapperAdditionExample-qt
                       OTBWrapperAddition
                       OTBWrapperQtWidget 
                       OTBWrapperCore
                       ${QT_LIBRARIES})
