SET(MODEL_SRCS
Model/otbViewerModel.cxx
)

SET(CONTROLLER_SRCS
Controller/otbViewerController.cxx
)

SET(VIEW_FL_SRCS
View/otbViewerViewGroup.fl
)

SET(VIEW_SRCS
View/otbViewerViewGUI.cxx
)


INCLUDE_DIRECTORIES(
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/View
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Model
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Controller
  ${OTB_BINARY_DIR}/Code/Visualization/Viewer
)

# Take the list of GUI files, generate .cxx and .h from them and add
# them to the build process of the target

FLTK_WRAP_UI(otbViewer ${VIEW_FL_SRCS})

# Suppress warning Fl_RGB_Image deferencing pointeur
IF(CMAKE_COMPILER_IS_GNUCXX)
      SET_SOURCE_FILES_PROPERTIES(
            ${MODEL_SRCS} 
            ${CONTROLLER_SRCS} 
            ${VIEW_SRCS} 
            ${otbViewer_FLTK_UI_SRCS}
            otbViewer.cxx
	    PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_EXECUTABLE(otbViewer
                   otbViewer.cxx
                   ${VIEW_SRCS} ${CONTROLLER_SRCS} ${MODEL_SRCS}
                   ${otbViewer_FLTK_UI_SRCS})
TARGET_LINK_LIBRARIES(otbViewer OTBIO OTBCommon OTBVisualization)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbViewer
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)
