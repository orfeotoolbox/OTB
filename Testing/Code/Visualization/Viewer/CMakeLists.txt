IF( NOT OTB-Applications_DISABLE_CXX_TESTING )

SET(INPUTDATA ${OTB_DATA_ROOT}/Input)

#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(Viewer_TESTS ${CXX_TEST_PATH}/otbViewerTests)



# ------------ Model Tests ---------------------------

ADD_TEST(ivmTuModelTest ${Viewer_TESTS}
	otbViewerModelTest
	${INPUTDATA}/poupees.tif
	${INPUTDATA}/amst.png
)

ADD_TEST(ivmTuModelTest_JPEG2000Id ${Viewer_TESTS}
	otbViewerModelWithAddNumTest
	${INPUTDATA}/bretagne.j2k
        3
)

ADD_TEST(ivmTvViewTest ${Viewer_TESTS}
	otbViewerViewTest
	${INPUTDATA}/poupees.tif
)

ADD_TEST(ivmTvViewTest_JPEG2000Id ${Viewer_TESTS}
	otbViewerViewTest
	${INPUTDATA}/bretagne.j2k:3
)

SET(Viewer_SRCS
otbViewerTests.cxx
otbViewerModelTest.cxx
otbViewerViewTest.cxx
${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Model/otbViewerModel.cxx
${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Controller/otbViewerController.cxx
${OTB_SOURCE_DIR}/Code/Visualization/Viewer/View/otbViewerViewGUI.cxx
${OTB_BINARY_DIR}/Code/Visualization/Viewer/otbViewerViewGroup.cxx
)

INCLUDE_DIRECTORIES(
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/View
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Model
  ${OTB_SOURCE_DIR}/Code/Visualization/Viewer/Controller
  ${OTB_BINARY_DIR}/Code/Visualization/Viewer
)

OTB_ADD_EXECUTABLE(otbViewerTests "${Viewer_SRCS}" "OTBVisualization;OTBTesting")

ENDIF( NOT OTB-Applications_DISABLE_CXX_TESTING )

