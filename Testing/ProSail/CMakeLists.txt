IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

SET(EPSILON 0.01)

SET(PROSAIL_TESTS ${CXX_TEST_PATH}/otbProSailTests)


ADD_TEST(otbProspectReflTest ${PROSAIL_TESTS}
   --compare-ascii ${EPSILON} ${BASELINE_FILES}/prospect_refl.txt
                   ${TEMP}/refl.txt
        otbProspectReflTest
        30.0 #cab
        10.0 #car
        0.0 #CBrown 
        0.015 #Cw
        0.009 #Cm
        1.2 #N
        ${TEMP}/refl.txt
        )

ADD_TEST(otbProspectTransTest ${PROSAIL_TESTS}
   --compare-ascii ${EPSILON} ${BASELINE_FILES}/prospect_trans.txt
                   ${TEMP}/trans.txt
        otbProspectTransTest
        30.0 #cab
        10.0 #car
        0.0 #CBrown 
        0.015 #Cw
        0.009 #Cm
        1.2 #N
        ${TEMP}/trans.txt
        )

ADD_TEST(otbSailReflHTest ${PROSAIL_TESTS}
   --compare-ascii ${EPSILON} ${BASELINE_FILES}/sail_resh.txt
                   ${TEMP}/resh.txt
        otbSailReflHTest
        30.0 #cab
        10.0 #car
        0.0 #CBrown 
        0.015 #Cw
        0.009 #Cm
        1.2 #N
        2 #LAI
        50 #Angl
        1 #PSoil 
        70 #Skyl
        0.2 #HSpot
        30 #TTS
        0 #TTO
        0 #PSI
        ${TEMP}/resh.txt
        )
        
ADD_TEST(otbSailReflVTest ${PROSAIL_TESTS}
   --compare-ascii ${EPSILON} ${BASELINE_FILES}/sail_resv.txt
                   ${TEMP}/resv.txt
        otbSailReflVTest
        30.0 #cab
        10.0 #car
        0.0 #CBrown 
        0.015 #Cw
        0.009 #Cm
        1.2 #N
        2 #LAI
        50 #Angl
        1 #PSoil 
        70 #Skyl
        0.2 #HSpot
        30 #TTS
        0 #TTO
        0 #PSI
        ${TEMP}/resv.txt
        )        
        
SET(PROSAIL_SRCS
otbProSailTests.cxx
otbProspectReflTest.cxx
otbProspectTransTest.cxx
otbSailReflHTest.cxx
otbSailReflVTest.cxx
)


ADD_EXECUTABLE(otbProSailTests ${PROSAIL_SRCS})
TARGET_LINK_LIBRARIES(otbProSailTests OTBIO OTBCommon OTBTesting PROSAIL)
