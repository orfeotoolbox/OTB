IF( NOT OTB-Applications_DISABLE_CXX_TESTING )

SET(INPUTDATA ${OTB_DATA_ROOT}/Input)

#Tolerance sur diff pixel image
SET(TOL 0.0)

IF(OTB_USE_VISU_GUI)

SET(Viewer_TESTS ${CXX_TEST_PATH}/otbViewerTests)



# ------------ Model Tests ---------------------------

ADD_TEST(ivmTuModelTest ${ImageViewer_TESTS}
	otbImageViewerModelTest
	${INPUTDATA}/poupees.tif
	${INPUTDATA}/amst.png
)

ADD_TEST(ivmTuModelTest_WtihJEPG2000Id ${ImageViewer_TESTS}
	otbViewerModelWithAddNumTest
	${INPUTDATA}/bretagne.j2k
        5
)

ADD_TEST(ivmTvViewTest ${ImageViewer_TESTS}
	otbImageViewerViewTest
	${INPUTDATA}/poupees.tif
)

ADD_TEST(ivmTvViewTest_WtihJEPG2000 ${ImageViewer_TESTS}
	otbImageViewerViewTest
	${INPUTDATA}/bretagne.j2k:3
)

SET(Viewer_SRCS
otbViewerTests.cxx
otbViewerModelTest.cxx
otbViewerViewTest.cxx
${OTB_SOURCE_DIR}/Code/Visualization/Viewer/View/otbViewerViewGUI.cxx
${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Model/otbViewerModel.cxx
${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Controller/otbViewerController.cxx
${OTB_BINARY_DIR}/Code/Visualization/Viewer/otbViewerViewGroup.cxx
)

INCLUDE_DIRECTORIES(
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/View
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Model
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Controller
  ${OTB_BINARY_DIR}/Code/Visualization/Viewer
)

# Suppress warning Fl_RGB_Image deferencing pointeur
#IF(CMAKE_COMPILER_IS_GNUCXX)
#      SET_SOURCE_FILES_PROPERTIES(
#             ${Viewer_SRCS}
#	     otbViewerTests.cxx
#            PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
#ENDIF(CMAKE_COMPILER_IS_GNUCXX)



OTB_ADD_EXECUTABLE(otbViewerTests "${Viewer_SRCS}" "OTBVisualization;OTBTesting")


ENDIF(OTB_USE_VISU_GUI)

ENDIF( NOT OTB-Applications_DISABLE_CXX_TESTING )

