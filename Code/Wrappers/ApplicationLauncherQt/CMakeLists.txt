INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(../QtWidget)

SET( SRCS
     otbApplicationLauncherQt.cxx )

ADD_EXECUTABLE(otbApplicationLauncherQt ${SRCS})

TARGET_LINK_LIBRARIES(otbApplicationLauncherQt
                       OTBWrapperQtWidget
                       OTBWrapperCore
                       ${QT_LIBRARIES})
