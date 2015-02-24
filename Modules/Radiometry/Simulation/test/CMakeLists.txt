otb_module_test()

#Paths for spectral libraries
set(SPECTRUM_DB ${INPUTDATA}/BD-CNES)
set(SRCLASS0 ${SPECTRUM_DB}/JHU/becknic/manmade/txt)
set(SRCLASS1 ${SPECTRUM_DB}/JHU/becknic/water/txt)
set(SRCLASS2 ${SPECTRUM_DB}/JHU/becknic/soils/txt)
set(SRCLASS3 ${SPECTRUM_DB}/JHU/becknic/vegetation/txt)
set(SRCLASS4 ${SPECTRUM_DB}/JHU/becknic/rocks/sedimentary/powder/0_75/txt)

set(OTBSimulationTests
otbSimulationTestDriver.cxx
otbReduceSpectralResponseSVMClassifier.cxx
otbProspectTransTest.cxx
otbReduceSpectralResponseClassifierRAndNIR.cxx
otbSpectralResponseNew.cxx
otbSatelliteRSR.cxx
otbSurfaceReflectanceToReflectanceFilterNew.cxx
otbImageSimulationMethodWithSpatialisationTest.cxx
otbImageSimulationMethodSVMClassif.cxx
otbReduceSpectralResponseNew.cxx
otbReduceSpectralResponse.cxx
otbAtmosphericEffects.cxx
otbProspectReflTest.cxx
otbSpatialisationTestNew.cxx
otbLabelMapToSimulatedImageFilterTest.cxx
otbImageSimulationMethodKMeansClassif.cxx
otbSpectralResponse.cxx
otbAtmosphericCorrectionsRSRSVMClassifier.cxx
otbSpatialisationTest.cxx
otbImageSimulationMethodWithVectorDataTest.cxx
otbSailReflVTest.cxx
otbSatelliteRSRNew.cxx
otbSailReflHTest.cxx
otbFilterFunctionValues.cxx
)

add_executable(otbSimulationTestDriver ${OTBSimulationTests})
target_link_libraries(otbSimulationTestDriver ${OTBSimulation-Test_LIBRARIES})
otb_module_target_label(otbSimulationTestDriver)

# Tests Declaration

otb_add_test(NAME siTuRSRSVMClassifier COMMAND otbSimulationTestDriver
  otbReduceSpectralResponseSVMClassifier
  ${SRCLASS0}
  ${SRCLASS1}
  ${SRCLASS2}
  ${SRCLASS3}
  ${SRCLASS4}
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nbBands
  0.1 #percentage of validation files
  )

otb_add_test(NAME siTvProspectTransTest COMMAND otbSimulationTestDriver
  --compare-ascii ${EPSILON_PROSPECT} ${BASELINE_FILES}/Simulation/siTvProspectTransTest.txt
  ${TEMP}/siTvProspectTransTest.txt
  otbProspectTransTest
  30.0 #cab
  10.0 #car
  0.0 #CBrown
  0.015 #Cw
  0.009 #Cm
  1.2 #N
  ${TEMP}/siTvProspectTransTest.txt
  )

otb_add_test(NAME siTuReduceSpectralResponseClassifierRAndNIR COMMAND otbSimulationTestDriver
  otbReduceSpectralResponseClassifierRAndNIR
  ${SPECTRUM_DB}/JHU/becknic/manmade/txt/0092uuu.txt
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  1 #red index
  2 #NIR index
  )

otb_add_test(NAME siTuSpectralResponseNew COMMAND otbSimulationTestDriver
  otbSpectralResponseNew
  )

otb_add_test(NAME siTuSatelliteRSRCheckValue COMMAND otbSimulationTestDriver
  otbSatelliteRSRCheckValue
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  0.50 #lambda
  0 #num band
  0.557 #expected
  )

otb_add_test(NAME siTuSatelliteRSRSolarIrradianceCheckValue2 COMMAND otbSimulationTestDriver
  otbSatelliteRSRSolarIrradianceCheckValue
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  1.7525000e+00  #lambda
  1.8526000e+02 #expected
  )      

otb_add_test(NAME siTuSatelliteRSRSolarIrradianceCheckValue COMMAND otbSimulationTestDriver
  otbSatelliteRSRSolarIrradianceCheckValue
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  0.45 #lambda
  2.0573899e3 #expected
  )      

otb_add_test(NAME siTuSatelliteRSR COMMAND otbSimulationTestDriver
  otbSatelliteRSR
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  2.5 #lambda
  2 #num band
  )

otb_add_test(NAME siTuSatelliteRSRCheckValue2 COMMAND otbSimulationTestDriver
  otbSatelliteRSRCheckValue
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  0.68 #lambda
  1 #num band
  0.717 #expected
  )            

otb_add_test(NAME siTuReflectanceToSurfaceReflectanceFilterNew COMMAND otbSimulationTestDriver
  otbSurfaceReflectanceToReflectanceFilterNew )

otb_add_test(NAME siTuImageSimulationMethodWithSpatialisationTest COMMAND otbSimulationTestDriver
  otbImageSimulationMethodWithSpatialisationTest
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb bands
  ${SPECTRUM_DB}/ #root path of spectrum
  3 #prolate radius
  ${TEMP}/ImageSimulationMethodWithSpatialisationTest.tif
  ${TEMP}/LabelImageSimulationMethodWithSpatialisationTest.tif
  )

otb_add_test(NAME siTuReduceSpectralResponseNew COMMAND otbSimulationTestDriver
  otbReduceSpectralResponseNew
  )

otb_add_test(NAME siTuReduceSpectralResponseReflectanceMode COMMAND otbSimulationTestDriver
  otbReduceSpectralResponse
  ${SPECTRUM_DB}/jpl/beckman/minerals/Arsenate/txt/A01Ac.txt
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  1 #reflectance mode
  )

otb_add_test(NAME siTuReduceSpectralResponse COMMAND otbSimulationTestDriver
  otbReduceSpectralResponse
  ${SPECTRUM_DB}/jpl/beckman/minerals/Arsenate/txt/A01Ac.txt
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb band
  0 #reflectance mode
  )

otb_add_test(NAME siTuReduceSpectralResponseSimpleValues COMMAND otbSimulationTestDriver
  otbReduceSpectralResponseSimpleValues
  ${TEMP}/siTuReduceSpectralResponseSimpleValuesRSRLum.txt
  0 # reflectance mode
  )

otb_add_test(NAME siTuReduceSpectralResponseSimpleValuesReflectance COMMAND otbSimulationTestDriver
  otbReduceSpectralResponseSimpleValues
  ${TEMP}/siTuReduceSpectralResponseSimpleValuesRSRRefl.txt
  1 # reflectance mode
  )

otb_add_test(NAME siTvReduceSpectralResponseReflectanceMode COMMAND otbSimulationTestDriver
  otbReduceSpectralResponse
  ${INPUTDATA}/SpectralResponse1.txt
  ${INPUTDATA}/Radiometry/Test/rep6S.dat
  4 #nb band
  1 #reflectance mode
  ${TEMP}/siTvReduceSpectralResponseReflectanceMode.txt
  )      

otb_add_test(NAME siTvProspectReflTest COMMAND otbSimulationTestDriver
  --compare-ascii ${EPSILON_PROSPECT} ${BASELINE_FILES}/Simulation/siTvProspectReflTest.txt
  ${TEMP}/siTvProspectReflTest.txt
  otbProspectReflTest
  30.0 #cab
  10.0 #car
  0.0 #CBrown
  0.015 #Cw
  0.009 #Cm
  1.2 #N
  ${TEMP}/siTvProspectReflTest.txt
  )

otb_add_test(NAME siTuSpatialisationTestNew COMMAND otbSimulationTestDriver
  otbSpatialisationTestNew
  )

otb_add_test(NAME siTuLabelMapToSimulatedImageFilterTest COMMAND otbSimulationTestDriver
  otbLabelMapToSimulatedImageFilterTest
  ${INPUTDATA}/Simulation/Shapefile/culturesWithAndWithoutPath.shp
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb bands
  ${SPECTRUM_DB}/jpl/beckman/minerals/ #root path of spectrum
  ${TEMP}/LabelMapToSimulatedImageFilterTest.tif
  )

otb_add_test(NAME siTuImageSimulationMethodKMeansClassif COMMAND otbSimulationTestDriver
  otbImageSimulationMethodKMeansClassif
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb bands
  ${SPECTRUM_DB}/ #root path of spectrum
  3 #prolate radius
  ${TEMP}/siTuImageSimulationMethodKMeansClassif.tif
  ${TEMP}/siTuKMeansClassifLabelImage.tif
  )

otb_add_test(NAME siTuSpectralResponse COMMAND otbSimulationTestDriver
  otbSpectralResponse
  ${SPECTRUM_DB}/JHU/becknic/manmade/txt/0092uuu.txt
  2.5 #lambda
  )

otb_add_test(NAME siTvSpatialisationTest COMMAND otbSimulationTestDriver
  --compare-image ${NOTOL} ${BASELINE}/Simulation/siTvSpatialisationTest.tif
  ${TEMP}/siTvSpatialisationTest.tif
  otbSpatialisationTest
  ${TEMP}/siTvSpatialisationTest.tif
  )

otb_add_test(NAME siTuImageSimulationMethodWithVectorDataTest COMMAND otbSimulationTestDriver
  otbImageSimulationMethodWithVectorDataTest
  ${INPUTDATA}/Simulation/Shapefile/stickPolygons_small.shp
  ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
  4 #nb bands
  ${SPECTRUM_DB}/ #root path of spectrum
  3 #prolate radius
  ${TEMP}/ImageSimulationMethodTest.tif
  ${TEMP}/LabelImageSimulationMethodTest.tif
  )

otb_add_test(NAME siTvSailReflVTest COMMAND otbSimulationTestDriver
  --compare-ascii ${EPSILON_PROSPECT} ${BASELINE_FILES}/Simulation/siTvSailReflVTest.txt
  ${TEMP}/siTvSailReflVTest.txt
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
  ${TEMP}/siTvSailReflVTest.txt
  )

otb_add_test(NAME siTuSatelliteRSRNew COMMAND otbSimulationTestDriver
  otbSatelliteRSRNew
  )

otb_add_test(NAME siTvSailReflHTest COMMAND otbSimulationTestDriver
  --compare-ascii ${EPSILON_PROSPECT} ${BASELINE_FILES}/Simulation/siTvSailReflHTest.txt
  ${TEMP}/siTvSailReflHTest.txt
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
  ${TEMP}/siTvSailReflHTest.txt
  )

otb_add_test(NAME raTvFilterFunctionValuesSpectralResponseTest COMMAND otbSimulationTestDriver
  --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raTvFilterFunctionValuesSpectralResponseTest.txt
  ${TEMP}/raTvFilterFunctionValuesSpectralResponseTest.txt
  otbFilterFunctionValuesSpectralResponseTest
  ${SPECTRUM_DB}/JHU/becknic/manmade/txt/0092uuu.txt
  ${TEMP}/raTvFilterFunctionValuesSpectralResponseTest.txt
  0.0025
  )

otb_add_test(NAME raTuFilterFunctionValuesTest COMMAND otbSimulationTestDriver
  otbFilterFunctionValuesTest
  ${TEMP}/raTuFilterFunctionValuesTest.txt
  0.5             # lambda min
  0.7             # lambda max
  0.05            # user step
  0.2 0.5 0.3 0.9 0.8 # vector
  )
