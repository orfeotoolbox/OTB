#
# Current inforamtions:
# Version 2.84 released on April Fools' day, 2007.
# Download site : http://www.csie.ntu.edu.tw/~cjlin/libsvm/
#

project(OTBSVM)

# source files for otbsvm
set(OTBSVM_SRCS
svm.cpp
)

# otbsvm needs ITK include dirs (bad design! this is a circular dependency between otbsvm and OTB)
include_directories(${ITK_INCLUDE_DIRS})

add_library(otbsvm ${OTBSVM_SRCS})
target_link_libraries(otbsvm ${ITK_LIBRARIES})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbsvm PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbsvm
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(FILES ${__files}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsvm
    COMPONENT Development)
endif()

