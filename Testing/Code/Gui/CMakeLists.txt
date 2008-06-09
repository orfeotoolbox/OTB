 
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

SET(GUI_TESTS ${CXX_TEST_PATH}/otbGuiTests)

# -------            otb::System  -------------
ADD_TEST(guTuFltkFilterWatcherNew ${GUI_TESTS}  
        otbFltkFilterWatcherNew
        )



# -------       Fichiers sources CXX -----------------------------------
SET(Gui_SRCS
otbFltkFilterWatcherNew.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

# ${TIFF_LIBRARY}

ADD_EXECUTABLE(otbGuiTests otbGuiTests.cxx ${Gui_SRCS})
TARGET_LINK_LIBRARIES(otbGuiTests OTBIO OTBGui OTBVisu OTBCommon gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon fltk)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
