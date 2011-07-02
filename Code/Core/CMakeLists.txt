file(GLOB srcs "*.cxx")

add_library(OTBWrapperCore ${srcs})
target_link_libraries(OTBWrapperCore OTBCommon OTBIO)
install(TARGETS OTBWrapperCore
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/otbapp
        FILES_MATCHING PATTERN "*.h")
 