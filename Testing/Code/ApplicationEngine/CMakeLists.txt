if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

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

# String parameter class test
add_test(owTuStringParameter ${OTB_WRAPPER_TESTS}
    otbWrapperStringParameterNew
  )

add_test(owTvStringParameter ${OTB_WRAPPER_TESTS}
    otbWrapperStringParameterTest1
    "value1"
    "mykey"
    "my description"
  )

# String list parameter class test
add_test(owTuStringListParameter ${OTB_WRAPPER_TESTS}
    otbWrapperStringListParameterNew
  )

add_test(owTvStringListParameter ${OTB_WRAPPER_TESTS}
    otbWrapperStringListParameterTest1
    "value1"
    "value2"
    "value3"
    "mykey"
    "my description"
  )

# EmptyParameter class test
add_test(owTuEmptyParameter ${OTB_WRAPPER_TESTS}
	otbWrapperEmptyParameterNew
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


# Input vector data list parameter class test
add_test(owTuInputVectorDataListParameter ${OTB_WRAPPER_TESTS}
    otbWrapperInputVectorDataListParameterNew
  )

add_test(owTvInputVectorDataListParameter ${OTB_WRAPPER_TESTS}
    otbWrapperInputVectorDataListParameterTest1
    ${INPUTDATA}/ToulousePoints-examples.shp
    ${INPUTDATA}/ToulouseRoads-examples.shp
    "mykey"
    "my description"
  )


# Output image parameter class test
add_test(owTuOutputImageParameter ${OTB_WRAPPER_TESTS}
    otbWrapperOutputImageParameterNew
  )

add_test(owTvOutputImageParameter ${OTB_WRAPPER_TESTS}
    otbWrapperOutputImageParameterTest1
    ${INPUTDATA}/poupees.tif
    ${TEMP}/owTvOutputImageParameter2.tif
    "mykey"
    "my description"
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

# Parameter RAM class test
add_test(owTvParameterRAM ${OTB_WRAPPER_TESTS}
    otbWrapperRAMParameterNew
  )

# ApplicationHtmlDocGenerator
add_test(owTvApplicationHtmlDocGeneratorNew ${OTB_WRAPPER_TESTS}
    otbWrapperApplicationHtmlDocGeneratorNew
  )


if (BUILD_APPLICATIONS)
add_test(NAME owTvApplicationHtmlDocGeneratorTest
         COMMAND ${OTB_WRAPPER_TESTS}
                  --compare-ascii ${NOTOL}
                    ${BASELINE}/owTvOReadImageInfoDoc.html
                    ${TEMP}/owTvOReadImageInfoDoc.html
                  otbWrapperApplicationHtmlDocGeneratorTest1
                    ReadImageInfo
                    $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                    ${TEMP}/owTvOReadImageInfoDoc.html
                    0 # don't show key in the parameterdescription
)
endif (BUILD_APPLICATIONS)

# DocExampleStructure
add_test(owTuDocExampleStructureNew ${OTB_WRAPPER_TESTS}
    otbWrapperDocExampleStructureNew
  )

add_test(owTuDocExampleStructureTest ${OTB_WRAPPER_TESTS}
  --compare-ascii ${NOTOL}
  ${BASELINE}/owTuDocExampleStructureTest.txt
  ${TEMP}/owTuDocExampleStructureTest.txt
  otbWrapperDocExampleStructureTest
  ${TEMP}/owTuDocExampleStructureTest.txt
 )




# ----------------Source files CXX -----------------------------------

set(Wrapper_SRCS
otbWrapperCoreTests.cxx

otbWrapperApplicationTest.cxx
otbWrapperChoiceParameterTest.cxx
otbWrapperEmptyParameterTest.cxx
otbWrapperInputImageParameterTest.cxx
otbWrapperInputVectorDataParameterTest.cxx
otbWrapperInputImageListParameterTest.cxx
otbWrapperInputVectorDataListParameterTest.cxx
otbWrapperOutputImageParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperParameterListTest.cxx
otbWrapperParameterTest.cxx
otbWrapperApplicationRegistryTest.cxx
otbWrapperParameterKeyTest.cxx
otbWrapperStringParameterTest.cxx
otbWrapperRAMParameterTest.cxx
otbWrapperStringListParameterTest.cxx
otbWrapperApplicationHtmlDocGeneratorTest.cxx
otbWrapperDocExampleStructureTest.cxx
)

include_directories(${CMAKE_SOURCE_DIR}/Code/Core)
add_executable(otbWrapperTests ${Wrapper_SRCS})
target_link_libraries(otbWrapperTests OTBApplicationEngine OTBTesting)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )