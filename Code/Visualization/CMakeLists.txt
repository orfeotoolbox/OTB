# Sources of non-templated classes.

FILE(GLOB OTBVisualization_SRCS "*.cxx" )

FLTK_WRAP_UI(OTBVisualization
               otbImageWidgetPackedManager.fl
               otbImageWidgetSplittedManager.fl
               otbMsgReporterGUI.fl)

ADD_LIBRARY(OTBVisualization ${OTBVisualization_SRCS} ${OTBVisualization_FLTK_UI_SRCS})
TARGET_LINK_LIBRARIES (OTBVisualization OTBGui OTBCommon OTBIO ${OTB_VISU_GUI_LIBRARIES})
IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBVisualization PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS OTBVisualization
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/*.fl")
  INSTALL(FILES ${__files1} ${__files2}  ${__files3}
    ${CMAKE_CURRENT_BINARY_DIR}/otbImageWidgetPackedManager.h
    ${CMAKE_CURRENT_BINARY_DIR}/otbImageWidgetSplittedManager.h
    ${CMAKE_CURRENT_BINARY_DIR}/otbMsgReporterGUI.h
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Visualization
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

# Add the viewer manager compilation
ADD_SUBDIRECTORY(Viewer)