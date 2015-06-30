otb_module_test()

set(OTBImageNoiseTests
otbImageNoiseTestDriver.cxx
otbFrostFilter.cxx
otbLeeFilter.cxx
otbFrostFilterNew.cxx
)

add_executable(otbImageNoiseTestDriver ${OTBImageNoiseTests})
target_link_libraries(otbImageNoiseTestDriver ${OTBImageNoise-Test_LIBRARIES})
otb_module_target_label(otbImageNoiseTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvFrostFilter COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreFrost_poupees_05_05_01.tif
  ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
  otbFrostFilter
  ${INPUTDATA}/GomaAvant.png   #poupees.hdr
  ${TEMP}/bfFiltreFrost_poupees_05_05_01.tif
  05 05 0.1)

otb_add_test(NAME bfTvFiltreLee1CanalPoupees COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/bfFiltreLee_05_05_04.tif
  ${TEMP}/bfFiltreLee_05_05_04.tif
  otbLeeFilter
  ${INPUTDATA}/GomaAvant.png                  #poupees_1canal.c1.hdr
  ${TEMP}/bfFiltreLee_05_05_04.tif
  05 05 4.0)


otb_add_test(NAME bfTvFiltreLee COMMAND otbImageNoiseTestDriver
  --compare-image ${EPSILON_7}  ${BASELINE}/GomaLee.tif 
  ${TEMP}/bfFiltreLee_02_02_12.tif
  otbLeeFilter
  ${INPUTDATA}/GomaAvant.png    #poupees.hdr
  ${TEMP}/bfFiltreLee_02_02_12.tif
  02 02 12.0)

otb_add_test(NAME bfTuFrostFilterNew COMMAND otbImageNoiseTestDriver
  otbFrostFilterNew)

