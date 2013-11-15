# Sources of non-templated classes.

file(GLOB OTBVisualization_SRCS "*.cxx" )

fltk_wrap_ui(OTBVisualization
               otbImageWidgetPackedManager.fl
               otbImageWidgetSplittedManager.fl
               otbMsgReporterGUI.fl)

add_library(OTBVisualization ${OTBVisualization_SRCS} ${OTBVisualization_FLTK_UI_SRCS})
target_link_libraries(OTBVisualization OTBGui OTBCommon OTBIO ${OTB_VISU_GUI_LIBRARIES})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBVisualization PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBVisualization
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/*.fl")
  install(FILES ${__files1} ${__files2}  ${__files3}
    ${CMAKE_CURRENT_BINARY_DIR}/otbImageWidgetPackedManager.h
    ${CMAKE_CURRENT_BINARY_DIR}/otbImageWidgetSplittedManager.h
    ${CMAKE_CURRENT_BINARY_DIR}/otbMsgReporterGUI.h
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Visualization
    COMPONENT Development)
endif()

# Add the viewer manager compilation
add_subdirectory(Viewer)
