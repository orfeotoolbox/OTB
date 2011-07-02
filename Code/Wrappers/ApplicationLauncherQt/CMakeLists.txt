include_directories(${QT_INCLUDE_DIRS})
include_directories(../QtWidget)

set( SRCS
     otbApplicationLauncherQt.cxx )

add_executable(otbApplicationLauncherQt ${SRCS})

target_link_libraries(otbApplicationLauncherQt
                       OTBWrapperQtWidget
                       OTBWrapperCore
                       ${QT_LIBRARIES})

install(TARGETS otbApplicationLauncherQt
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
                       