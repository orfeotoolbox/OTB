cmake_minimum_required (VERSION 2.6)
project (projchambolle)

find_package(OTB REQUIRED)
include(${OTB_USE_FILE})


set(SOURCES otbContrastEnhancementFilter.cxx)

add_executable(Contrast otbContrastEnhancementFilter.cxx)

target_link_libraries(Contrast ${OTB_LIBRARIES})


# message(WARNING "Include dirs : ${OTB_INCLUDE_DIRS}")