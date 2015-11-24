##### check if standalone project ######
if(NOT PROJECT_NAME)
  if(WIN32)
    cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
  else()
    cmake_minimum_required(VERSION 2.8.6 FATAL_ERROR)
  endif()
  project(OTBExamples)
endif()

#check if this is a standalone build?

if(PROJECT_NAME STREQUAL "OTBExamples")
  if(BUILD_TESTING)
    include(CTest)
  endif()

  # dashboard build. Why do I need to see the deprecated warnings on dashboard!
  # It just fades the actual warnings and most importantly very annoying.
  if(NOT "$ENV{DASHBOARD_TEST_FROM_CTEST}" STREQUAL "")
    if(UNIX)
      if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cpp")
      else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-#warnings")
      endif() #CMAKE_COMPILER_IS_GNUCXX
    endif() #UNIX
  endif()

  # --------------------------------------------------------------------------------
  # Manage PHR data input
  if(OTB_DATA_USE_LARGEINPUT)
    set(GenericTestPHR_DATA_INPUTS
      ${GenericTestPHR_DATA_INPUTS}
      ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES-PRE/TLSE_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits/IMG_PHR1Z_PMS_N_001/IMG_PHR1A_PMS-N_201006181052297_ORT_IPU_20111011_0619-001_R1C1.JP2
      ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES-PRE/TLSE_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20111109_7807-004_R1C1.TIF
      ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF
      ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2
      ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R1C1.JP2
      ${OTB_DATA_LARGEINPUT_ROOT}/PLEIADES/PARIS_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits/IMGPHR_PHR1A_ORTHO_20120502_IPU_20120430_6347-003_V1/IMG_R1C1.JP2
      )

    set(GenericTestPHR_DATA_TYPES
      ${GenericTestPHR_DATA_TYPES}
      SIMUL_PHR_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits
      SIMUL_PHR_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits
      SIMUL_PHR_TIFF_PRIMARY_DIMAPv2_MS_lossy_12bits
      PHR_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits
      PHR_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits
      PHR_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits
      )

    set(GenericTestSPOT6_DATA_INPUTS
      ${GenericTestSPOT6_DATA_INPUTS}
      ${OTB_DATA_LARGEINPUT_ROOT}/SPOT6/SPOT6_Ortho_Bundle_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201306031242000_ORT_816005101_R1C1.JP2
      ${OTB_DATA_LARGEINPUT_ROOT}/SPOT6/SPOT6_Ortho_Pan-sharpened_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_PMS_001_A/IMG_SPOT6_PMS_201305251604372_ORT_816009101_R1C1.JP2
      ${OTB_DATA_LARGEINPUT_ROOT}/SPOT6/SPOT6_Primary_Bundle_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201211100138085_SEN_816007101_R1C1.JP2
      )

    set(GenericTestSPOT6_DATA_TYPES
      ${GenericTestSPOT6_DATA_TYPES}
      SPOT6_Ortho_Bundle_12_bits
      SPOT6_Ortho_Pan-sharpened_12_bits
      SPOT6_Primary_Bundle_12_bits
      )
  endif()


  ########################## copied from OTB/CMakeLists.txt ##########################

  #Generate cmake variables with a set of largeinput data used in tests
  set(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
  set(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
  set(INPUTDATA      ${OTB_DATA_ROOT}/Input)
  set(TEMP           ${CMAKE_BINARY_DIR}/Testing/Temporary)
  set(EXAMPLEDATA    ${OTB_DATA_ROOT}/Examples)
  set(OTBAPP_BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Images)
  set(OTBAPP_BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Files)

  set(NOTOL      0.0)
  set(EPSILON_1  0.1)
  set(EPSILON_2  0.01)
  set(EPSILON_3  0.001)
  set(EPSILON_4  0.0001)
  set(EPSILON_5  0.00001)
  set(EPSILON_6  0.000001)
  set(EPSILON_7  0.0000001)
  set(EPSILON_8  0.00000001)
  set(EPSILON_9  0.000000001)
  set(EPSILON_10 0.0000000001)
  set(EPSILON_11 0.00000000001)
  set(EPSILON_12 0.000000000001)
  set(EPSILON_13 0.0000000000001)
  set(EPSILON_14 0.00000000000001)
  set(EPSILON_15 0.000000000000001)

  #-----------------------------------------------------------------------------
  # OTB wrapper for add_test that automatically sets the test's LABELS property
  # to the value of its containing module.
  #
  function(otb_add_test)
    set(largeinput_regex "LARGEINPUT{([^;{}\r\n]*)}")
    set(_depends_on_largeinput OFF)
    foreach(arg IN LISTS ARGN)
      if("x${arg}" MATCHES "${largeinput_regex}")
        string(REGEX REPLACE "${largeinput_regex}" "\\1" largeinput_relative_path "${arg}")
        set(_fullpath "${OTB_DATA_LARGEINPUT_ROOT}/${largeinput_relative_path}")
        list(APPEND _out_arg ${_fullpath})
        set(_depends_on_largeinput ON)
      else()
        list(APPEND _out_arg ${arg})
      endif()
    endforeach()
    if (_depends_on_largeinput AND NOT OTB_DATA_USE_LARGEINPUT)
      return()
    endif()
    add_test(${_out_arg})
    if("NAME" STREQUAL "${ARGV0}")
      set(_iat_testname ${ARGV1})
    else()
      set(_iat_testname ${ARGV0})
    endif()
    if(otb-module)
      set(_label ${otb-module})
    else()
      set(_label ${main_project_name})
    endif()
    set_property(TEST ${_iat_testname} PROPERTY LABELS ${_label})
  endfunction()

  ########################## end copied from OTB/CMakeLists.txt ##########################
else()

  #Examples that require large input cannot be enabled when building as standalone
  include(${CMAKE_CURRENT_SOURCE_DIR}/../CMake/OTBManageLargeInputPaths.cmake)
endif()

find_package(OTB REQUIRED)
include(${OTB_USE_FILE})
message(STATUS "Found OTB: ${OTB_USE_FILE}")

set(OTB_TEST_DRIVER otbTestDriver)

if(OTBApplicationEngine_LOADED)
  add_subdirectory(Application)
endif()
add_subdirectory(BasicFilters)
add_subdirectory(ChangeDetection)
add_subdirectory(Classification)
add_subdirectory(DataRepresentation)
add_subdirectory(DimensionReduction)
add_subdirectory(DisparityMap)
add_subdirectory(FeatureExtraction)
add_subdirectory(Filtering)
add_subdirectory(Fusion)
add_subdirectory(Hyperspectral)
add_subdirectory(Installation)
add_subdirectory(IO)
add_subdirectory(Iterators)
add_subdirectory(Learning)
add_subdirectory(Markov)
add_subdirectory(MultiScale)
add_subdirectory(OBIA)
add_subdirectory(Patented)
add_subdirectory(Projections)
add_subdirectory(Radiometry)
add_subdirectory(Registration)
add_subdirectory(Segmentation)
add_subdirectory(Simulation)
add_subdirectory(Tutorials)
