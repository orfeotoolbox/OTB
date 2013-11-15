project(ApplicationExamples)
include_regular_expression("^.*$")


OTB_CREATE_APPLICATION(NAME Example
                       SOURCES ApplicationExample.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)
set(EXAMPLEDATA ${OTB_DATA_ROOT}/Examples)

OTB_TEST_APPLICATION(NAME apTeGenerateAnApplicationExample
                     APP  Example
                     OPTIONS -in ${EXAMPLEDATA}/QB_Suburb.png
                			 -out ${TEMP}/apTeGenerateAnApplicationExampleOutput.png)

endif()
