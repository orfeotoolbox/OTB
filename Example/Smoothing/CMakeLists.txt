
SET( SRCS
     otbSmoothing.cxx )

ADD_LIBRARY(OTBWrapperSmoothing otbSmoothing.cxx)
TARGET_LINK_LIBRARIES(OTBWrapperSmoothing OTBWrapperCore)

ADD_EXECUTABLE(otbWrapperSmoothingExample-qt otbSmoothingQt.cxx)
TARGET_LINK_LIBRARIES(otbWrapperSmoothingExample-qt
                       OTBWrapperSmoothing
                       OTBWrapperQtWidget 
                       OTBWrapperCore
                       ${QT_LIBRARIES})
