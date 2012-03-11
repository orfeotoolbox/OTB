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

ADD_TEST(ivmTuModelTest_HDFId ${Viewer_TESTS}
	otbViewerModelWithAddNumTest
	${INPUTDATA}/GSSTF_NCEP.2b.2008.12.31.he5
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

ADD_TEST(ivmTvViewTest_HDFId ${Viewer_TESTS}
	otbViewerViewTest
	${INPUTDATA}/GSSTF_NCEP.2b.2008.12.31.he5:3
)

# We need to regenerate the UI sources, even though FLTK_WRAP_UI is called 
# in Code/Visualization/Viewer/CMakeLists.txt
# This is because the file is generated at build time,
# so we cannot include it in Viewer_SRCS
FLTK_WRAP_UI(otbViewerTests
             ../../../../Code/Visualization/Viewer/View/otbViewerViewGroup.fl )

SET(Viewer_SRCS
otbViewerTests.cxx
otbViewerModelTest.cxx
otbViewerViewTest.cxx
../../../../Code/Visualization/Viewer/Model/otbViewerModel.cxx
../../../../Code/Visualization/Viewer/Controller/otbViewerController.cxx
../../../../Code/Visualization/Viewer/View/otbViewerViewGUI.cxx
${otbViewerTests_FLTK_UI_SRCS}
)

INCLUDE_DIRECTORIES(
  ../../../../Code/Visualization/Viewer/View
  ../../../../Code/Visualization/Viewer/Model
  ../../../../Code/Visualization/Viewer/Controller
  ${OTB_BINARY_DIR}/Code/Visualization/Viewer
)

ADD_EXECUTABLE(otbViewerTests ${Viewer_SRCS})
TARGET_LINK_LIBRARIES(otbViewerTests OTBVisualization OTBTesting)
