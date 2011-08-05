set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

set(INPUTDATA ${OTB_DATA_ROOT}/Input)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

set(OTB_WRAPPER_TESTS ${CXX_TEST_PATH}/otbWrapperTests)


add_test(owTuParameterNew ${OTB_WRAPPER_TESTS}
	otbWrapperParameterNew
  )

add_test(owTvParameter ${OTB_WRAPPER_TESTS}
	otbWrapperParameterTest1
	"param1"
  )


# NumericalParameter class test
add_test(owTuNumericalParameter ${OTB_WRAPPER_TESTS}
	otbWrapperNumericalParameterNew
  )

# EmptyParameter class test
add_test(owTuEmptyParameter ${OTB_WRAPPER_TESTS}
	otbWrapperEmptyParameterNew
  )

# NumericalParameter class test
add_test(owTuNumericalParameter ${OTB_WRAPPER_TESTS}
	otbWrapperNumericalParameterNew
  )

add_test(owTvNumericalParameter ${OTB_WRAPPER_TESTS}
	otbWrapperNumericalParameterTest1
	42.42
	"mykey"
	"my description"
  )

# Input Image parameter class test
add_test(owTuInputImageParameter ${OTB_WRAPPER_TESTS}
	otbWrapperInputImageParameterNew
  )
add_test(owTvInputImageParameter ${OTB_WRAPPER_TESTS}
	otbWrapperInputImageParameterTest1
        ${INPUTDATA}/poupees.tif
	"mykey"
	"my description"
  )

# Input VectorData parameter class test
add_test(owTuInputVectorDataParameter ${OTB_WRAPPER_TESTS}
    otbWrapperInputVectorDataParameterNew
  )

# Application class test
add_test(owTuApplication ${OTB_WRAPPER_TESTS}
	otbWrapperApplicationNew
  )

# Application class test
add_test(owTvApplicationRegistry ${OTB_WRAPPER_TESTS}
    otbWrapperApplicationRegistry
  )

# ParameterKey class test
add_test(owTvParameterKey ${OTB_WRAPPER_TESTS}
    otbWrapperParameterKey
  )

# Input image list parameter class test
add_test(owTuInputImageListParameter ${OTB_WRAPPER_TESTS}
    otbWrapperInputImageListParameterNew
  )

add_test(owTvInputImageListParameter ${OTB_WRAPPER_TESTS}
    otbWrapperInputImageListParameterTest1
    ${INPUTDATA}/poupees_c1.hdr
    ${INPUTDATA}/poupees.tif
    "mykey"
    "my description"
  )


# ----------------Source files CXX -----------------------------------

set(Wrapper_SRCS
otbWrapperCoreTests.cxx

otbWrapperApplicationTest.cxx
otbWrapperChoiceParameterTest.cxx
otbWrapperEmptyParameterTest.cxx
otbWrapperInputImageParameterTest.cxx
otbWrapperInputVectorDataParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperOutputImageParameterTest.cxx
otbWrapperParameterListTest.cxx
otbWrapperParameterTest.cxx
otbWrapperApplicationRegistryTest.cxx
otbWrapperParameterKeyTest.cxx
otbWrapperInputImageListParameterTest.cxx
)

include_directories(${CMAKE_SOURCE_DIR}/Code/Core)
add_executable(otbWrapperTests ${Wrapper_SRCS})
target_link_libraries(otbWrapperTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting OTBWrapperCore)
