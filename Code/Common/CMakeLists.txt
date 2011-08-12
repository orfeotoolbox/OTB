file(GLOB srcs "*.cxx")

add_library(OTBWrapperCommon ${srcs})
target_link_libraries(OTBWrapperCommon OTBCommon OTBIO)
install(TARGETS OTBWrapperCommon
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/otbapp
        FILES_MATCHING PATTERN "*.h")
 