set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(EXAMPLEDATA ${OTB_DATA_ROOT}/Examples)

if(OTB_DATA_USE_LARGEINPUT)
  set(LARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(BASELINE              ${OTB_DATA_ROOT}/Baseline/OTB/Images)
set(BASELINE_FILES        ${OTB_DATA_ROOT}/Baseline/OTB/Files)

set(OTBAPP_BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Images)
set(OTBAPP_BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Files)

set(NOTOL 0.0)
set(EPSILON_3 0.001)
set(EPSILON_5 0.00001)
set(EPSILON_7 0.0000001)
set(EPSILON_9 0.000000001)
set(EPSILON_12 0.000000000001)

set(OTB_WRAPPER_APPLICATION_TESTS ${CXX_TEST_PATH}/otbWrapperApplicationDocTests)

set(WrapperApplication_SRCS
    otbWrapperApplicationTests1.cxx
    otbWrapperApplicationDocTests.cxx
    )

include_directories(${CMAKE_SOURCE_DIR}/Code/Core)
include_directories(${CMAKE_SOURCE_DIR}/Code/Common)

add_executable(otbWrapperApplicationDocTests ${WrapperApplication_SRCS})
target_link_libraries(otbWrapperApplicationDocTests
                        OTBIO
                        OTBCommon
                        OTBTesting
                        OTBApplicationEngine)


foreach(app ${OTB_APPLICATIONS_NAME_LIST})

add_test(NAME apTvDocAppli_${app}
         COMMAND ${OTB_WRAPPER_APPLICATION_TESTS}
           otbWrapperApplicationDocTest
           ${app}
           $<TARGET_FILE_DIR:otbapp_${app}> )

endforeach(app)

add_subdirectory(CartographicDBValidation)
add_subdirectory(ChangeDetection)
add_subdirectory(Classification)
add_subdirectory(DimensionalityReduction)
add_subdirectory(DisparityMap)
add_subdirectory(FeatureExtraction)
add_subdirectory(Fusion)
add_subdirectory(Hyperspectral)
add_subdirectory(Projections)
add_subdirectory(Radiometry)
add_subdirectory(Rasterization)
add_subdirectory(Segmentation)
add_subdirectory(Utils)

