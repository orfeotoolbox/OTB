IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

SET(PROSAIL_TESTS ${CXX_TEST_PATH}/otbProSailTests)

ADD_TEST(otbProSailNew ${PROSAIL_TESTS}
        otbProSailNew
        )

ADD_TEST(otbProspectTest ${PROSAIL_TESTS}
        otbProspectTest
        )
        
ADD_TEST(otbSailTest ${PROSAIL_TESTS}
        otbSailTest
        )
        
SET(PROSAIL_SRCS
otbProSailTests.cxx
otbProSailNew.cxx
otbProspectTest.cxx
otbSailTest.cxx
)


ADD_EXECUTABLE(otbProSailTests ${PROSAIL_SRCS})
TARGET_LINK_LIBRARIES(otbProSailTests OTBIO OTBCommon OTBTesting PROSAIL)
