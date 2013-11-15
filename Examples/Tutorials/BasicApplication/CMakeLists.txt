project(BasicApplication)

set(MODEL_SRCS
Model/otbBasicApplicationModel.cxx
)

set(CONTROLLER_SRCS
Controller/otbBasicApplicationController.cxx
)

set(VIEW_FL_SRCS
View/otbBasicApplicationViewGUI.fl
)

set(VIEW_SRCS
View/otbBasicApplicationView.cxx
)

set(COMMON_FL_SRCS
Common/otbMsgReporterGUI.fl
)

set(COMMON_SRCS
Common/otbMsgReporter.cxx
)

include_directories(
  ${BasicApplication_SOURCE_DIR}/View
  ${BasicApplication_SOURCE_DIR}/Model
  ${BasicApplication_SOURCE_DIR}/Controller
  ${BasicApplication_SOURCE_DIR}/Common
  ${BasicApplication_SOURCE_DIR}
)

fltk_wrap_ui(otbMVCBasicApplication ${VIEW_FL_SRCS} ${COMMON_FL_SRCS})


# Suppress warning Fl_RGB_Image deferencing pointeur
if(CMAKE_COMPILER_IS_GNUCXX)
      set_source_files_properties(
            ${VIEW_SRCS}
            ${CONTROLLER_SRCS}
            ${MODEL_SRCS}
            ${otbMVCBasicApplication_FLTK_UI_SRCS}
            otbBasicApplicationApplication.cxx
            PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
endif(CMAKE_COMPILER_IS_GNUCXX)


add_library(otbMVCBasicApplication ${VIEW_SRCS} ${otbMVCBasicApplication_FLTK_UI_SRCS} ${MODEL_SRCS} ${CONTROLLER_SRCS} ${COMMON_SRCS})
target_link_libraries(otbMVCBasicApplication OTBCommon OTBIO OTBVisualization)
install_targets(/bin otbMVCBasicApplication)
add_dependencies(otbMVCBasicApplication OTBApplicationsCommon)

add_executable(otbBasicApplication otbBasicApplication.cxx )
target_link_libraries(otbBasicApplication otbMVCBasicApplication)
install_targets(/bin otbBasicApplication)


