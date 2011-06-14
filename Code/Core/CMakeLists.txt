file(GLOB srcs "*.cxx")

add_library(OTBWrapperCore ${srcs})
target_link_libraries(OTBWrapperCore OTBCommon OTBIO)
