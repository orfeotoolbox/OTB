 
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

# To suppress "ld: cycle in dylib re-exports with /usr/X11R6/lib/libGL.dylib" error on APPLE configuration 
IF(APPLE)
      FOREACH(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
        SET(CMAKE_EXE_LINKER_FLAGS${c} "${CMAKE_EXE_LINKER_FLAGS${c}} -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
      ENDFOREACH(c)
ENDIF(APPLE)


ADD_EXECUTABLE(otbGuiTests otbGuiTests.cxx ${Gui_SRCS})
TARGET_LINK_LIBRARIES(otbGuiTests OTBIO OTBGui OTBVisu OTBCommon gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon fltk)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
