file(GLOB srcs "*.cxx")

add_library(OTBWrapperCommandLine ${srcs})
target_link_libraries(OTBWrapperCommandLine OTBWrapperCore) #OTBCommon OTBIO)
install(TARGETS OTBWrapperCommandLine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/otbapp
        FILES_MATCHING PATTERN "*.h")
                       