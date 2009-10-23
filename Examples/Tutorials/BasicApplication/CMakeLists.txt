PROJECT(BasicApplication)

SET(MODEL_SRCS
Model/otbBasicApplicationModel.cxx
)

SET(CONTROLLER_SRCS
Controller/otbBasicApplicationController.cxx
)

SET(VIEW_FL_SRCS
View/otbBasicApplicationViewGUI.fl
)

SET(VIEW_SRCS
View/otbBasicApplicationView.cxx
)

SET(COMMON_FL_SRCS
Common/otbMsgReporterGUI.fl
)

SET(COMMON_SRCS
Common/otbMsgReporter.cxx
)

INCLUDE_DIRECTORIES(
  ${BasicApplication_SOURCE_DIR}/View
  ${BasicApplication_SOURCE_DIR}/Model
  ${BasicApplication_SOURCE_DIR}/Controller
  ${BasicApplication_SOURCE_DIR}/Common
  ${BasicApplication_SOURCE_DIR}
)

FLTK_WRAP_UI(otbMVCBasicApplication ${VIEW_FL_SRCS} ${COMMON_FL_SRCS})


# Suppress warning Fl_RGB_Image deferencing pointeur
IF(CMAKE_COMPILER_IS_GNUCXX)
      SET_SOURCE_FILES_PROPERTIES(
            ${VIEW_SRCS}
            ${CONTROLLER_SRCS}
            ${MODEL_SRCS}
            ${otbMVCBasicApplication_FLTK_UI_SRCS}
            otbBasicApplicationApplication.cxx
            PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


ADD_LIBRARY(otbMVCBasicApplication ${VIEW_SRCS} ${otbMVCBasicApplication_FLTK_UI_SRCS} ${MODEL_SRCS} ${CONTROLLER_SRCS} ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(otbMVCBasicApplication OTBCommon OTBIO OTBVisualization)
INSTALL_TARGETS(/bin otbMVCBasicApplication)
ADD_DEPENDENCIES(otbMVCBasicApplication OTBApplicationsCommon)

ADD_EXECUTABLE(otbBasicApplication otbBasicApplication.cxx )
TARGET_LINK_LIBRARIES(otbBasicApplication otbMVCBasicApplication)
INSTALL_TARGETS(/bin otbBasicApplication)


