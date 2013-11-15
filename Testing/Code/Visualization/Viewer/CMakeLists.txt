set(INPUTDATA ${OTB_DATA_ROOT}/Input)

#Tolerance sur diff pixel image
set(TOL 0.0)

set(Viewer_TESTS ${CXX_TEST_PATH}/otbViewerTests)



# ------------ Model Tests ---------------------------

add_test(ivmTuModelTest ${Viewer_TESTS}
	otbViewerModelTest
	${INPUTDATA}/poupees.tif
	${INPUTDATA}/amst.png
)

add_test(ivmTuModelTest_JPEG2000Id ${Viewer_TESTS}
	otbViewerModelWithAddNumTest
	${INPUTDATA}/bretagne.j2k
        3
)

add_test(ivmTuModelTest_HDFId ${Viewer_TESTS}
	otbViewerModelWithAddNumTest
	${INPUTDATA}/GSSTF_NCEP.2b.2008.12.31.he5
        3
)

add_test(ivmTvViewTest ${Viewer_TESTS}
	otbViewerViewTest
	${INPUTDATA}/poupees.tif
)

add_test(ivmTvViewTest_JPEG2000Id ${Viewer_TESTS}
	otbViewerViewTest
	${INPUTDATA}/bretagne.j2k:3
)

add_test(ivmTvViewTest_HDFId ${Viewer_TESTS}
	otbViewerViewTest
	${INPUTDATA}/GSSTF_NCEP.2b.2008.12.31.he5:3
)

# We need to regenerate the UI sources, even though FLTK_WRAP_UI is called
# in Code/Visualization/Viewer/CMakeLists.txt
# This is because the file is generated at build time,
# so we cannot include it in Viewer_SRCS
fltk_wrap_ui(otbViewerTests
             ../../../../Code/Visualization/Viewer/View/otbViewerViewGroup.fl )

set(Viewer_SRCS
otbViewerTests.cxx
otbViewerModelTest.cxx
otbViewerViewTest.cxx
../../../../Code/Visualization/Viewer/Model/otbViewerModel.cxx
../../../../Code/Visualization/Viewer/Controller/otbViewerController.cxx
../../../../Code/Visualization/Viewer/View/otbViewerViewGUI.cxx
${otbViewerTests_FLTK_UI_SRCS}
)

include_directories(
  ../../../../Code/Visualization/Viewer/View
  ../../../../Code/Visualization/Viewer/Model
  ../../../../Code/Visualization/Viewer/Controller
  ${OTB_BINARY_DIR}/Code/Visualization/Viewer
)

add_executable(otbViewerTests ${Viewer_SRCS})
target_link_libraries(otbViewerTests OTBVisualization OTBTesting)
