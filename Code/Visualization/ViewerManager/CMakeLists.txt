SET(MODEL_SRCS
Model/otbImageViewerManagerModel.cxx
)

SET(CONTROLLER_SRCS
Controller/otbImageViewerManagerController.cxx
)

SET(VIEW_FL_SRCS
View/otbImageViewerManagerViewGroup.fl
)

SET(VIEW_SRCS
View/otbImageViewerManagerViewGUI.cxx
)


INCLUDE_DIRECTORIES(
  ${OTB_SOURCE_DIR}/Code/Visualization/ViewerManager/View
  ${OTB_SOURCE_DIR}/Code/Visualization/ViewerManager/Model
  ${OTB_SOURCE_DIR}/Code/Visualization/ViewerManager/Controller
  ${OTB_BINARY_DIR}/Code/Visualization/ViewerManager
)

# Take the list of GUI files, generate .cxx and .h from them and add
# them to the build process of the target

FLTK_WRAP_UI(otbMVCImageViewerManager ${VIEW_FL_SRCS})

# Suppress warning Fl_RGB_Image deferencing pointeur
IF(CMAKE_COMPILER_IS_GNUCXX)
      SET_SOURCE_FILES_PROPERTIES(
            ${MODEL_SRCS} 
            ${CONTROLLER_SRCS} 
            ${VIEW_SRCS} 
            ${otbMVCImageViewerManager_FLTK_UI_SRCS}
            otbImageViewerManager.cxx
	    PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


ADD_LIBRARY(otbMVCImageViewerManager ${VIEW_SRCS} ${CONTROLLER_SRCS} ${MODEL_SRCS} ${otbMVCImageViewerManager_FLTK_UI_SRCS} ) 
TARGET_LINK_LIBRARIES(otbMVCImageViewerManager OTBIO OTBVisualization)
ADD_DEPENDENCIES(otbMVCImageViewerManager OTBCommon OTBCommon)

ADD_EXECUTABLE(otbImageViewerManager otbImageViewerManager.cxx )
TARGET_LINK_LIBRARIES(otbImageViewerManager otbMVCImageViewerManager)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbImageViewerManager otbMVCImageViewerManager
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)
