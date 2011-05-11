
#IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(SPECTRUM_DB ${RSR_DATA}/BD-CNES)
SET(TEMP ${CMAKE_CURRENT_BINARY_DIR}/../Temporary)

SET(SRCLASS0 ${SPECTRUM_DB}/JHU/becknic/manmade/txt)
SET(SRCLASS1 ${SPECTRUM_DB}/JHU/becknic/water/txt)
SET(SRCLASS2 ${SPECTRUM_DB}/JHU/becknic/soils/txt)
SET(SRCLASS3 ${SPECTRUM_DB}/JHU/becknic/vegetation/txt)
SET(SRCLASS4 ${SPECTRUM_DB}/JHU/becknic/rocks/sedimentary/powder/0_75/txt)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.00001)
SET(EPSILON_PROSPECT 0.01)
SET(Simulation_TESTS1 ${CXX_TEST_PATH}/otbSimulationTests1)
SET(Simulation_TESTS2 ${CXX_TEST_PATH}/otbSimulationTests2)
SET(Simulation_TESTS3 ${CXX_TEST_PATH}/otbSimulationTests3)
SET(Simulation_TESTS4 ${CXX_TEST_PATH}/otbSimulationTests4)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSimulation_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::reflectancetosurfacereflectance  --------------------------

ADD_TEST(maTuReflectanceToSurfaceReflectanceFilterNew ${Simulation_TESTS1}
        otbReflectanceToSurfaceReflectanceFilterNew )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSimulation_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(otbSpectralResponseNew ${Simulation_TESTS2}
        otbSpectralResponseNew
        )

ADD_TEST(otbSpectralResponse ${Simulation_TESTS2}
        otbSpectralResponse
        ${SPECTRUM_DB}/JHU/becknic/manmade/txt/0092uuu.txt
        2.5 #lambda
        )

ADD_TEST(otbSatelliteRSRNew ${Simulation_TESTS2}
        otbSatelliteRSRNew
        )

ADD_TEST(otbSatelliteRSR ${Simulation_TESTS2}
        otbSatelliteRSR
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb band
        2.5 #lambda
        2 #num band
        )

ADD_TEST(otbReduceSpectralResponseNew ${Simulation_TESTS2}
        otbReduceSpectralResponseNew
        )

ADD_TEST(otbReduceSpectralResponse ${Simulation_TESTS2}
        otbReduceSpectralResponse
        ${SPECTRUM_DB}/jpl/beckman/minerals/Arsenate/txt/A01Ac.txt
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb band
        )

ADD_TEST(otbReduceSpectralResponseClassifierRAndNIR ${Simulation_TESTS2}
        otbReduceSpectralResponseClassifierRAndNIR
        ${SPECTRUM_DB}/JHU/becknic/manmade/txt/0092uuu.txt
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb band
        1 #red index
        2 #NIR index
        )

#FIXME uncomment this test and make it compatible with filters able to read filter function values (rep6S.dat)        
# ADD_TEST(otbAtmosphericEffects ${Simulation_TESTS2}
#         otbAtmosphericEffects
#         ${SPECTRUM_DB}/JHU/becknic/manmade/txt/0092uuu.txt
#         ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
#         4 #nb band
#         4 #day
#         12 #month
#         27.3 #zenithSolarAngle
#         152.7 #azimutSolarAngle
#         2.5 #viewingZenitalAngle
#         -77.0 #viewingAzimutalAngle
#         1013. #atmoPressure
#         2.48134 #waterVaporAmount
#         0.34400 #ozoneAmount
#         1 #aerosolModelValue
#         0.199854 #aerosolOptical
#         )

ADD_TEST(otbSpatialisationTest ${Simulation_TESTS2}
        otbSpatialisationTest
        ${TEMP}/SpatialisationTest.tif
        )
        
ADD_TEST(otbLabelMapToSimulatedImageFilterTest ${Simulation_TESTS2}
        otbLabelMapToSimulatedImageFilterTest
        ${INPUTDATA}/Simulation/culturesWithAndWithoutPath.shp
#         /home2/arnaud/PROJETS/SimuCapteur/data/largeInput.shp
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb bands
        ${SPECTRUM_DB}/jpl/beckman/minerals/ #root path of spectrum
        ${TEMP}/LabelMapToSimulatedImageFilterTest.tif
        )
        
ADD_TEST(otbImageSimulationMethodWithVectorDataTest ${Simulation_TESTS2}
        otbImageSimulationMethodWithVectorDataTest
        ${INPUTDATA}/Simulation/stickPolygons.shp
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb bands
        ${SPECTRUM_DB}/ #root path of spectrum
        3 #prolate radius
        ${TEMP}/ImageSimulationMethodTest.tif
        ${TEMP}/LabelImageSimulationMethodTest.tif
        )
        
ADD_TEST(otbImageSimulationMethodWithSpatialisationTest ${Simulation_TESTS2}
        otbImageSimulationMethodWithSpatialisationTest
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb bands
        ${SPECTRUM_DB}/ #root path of spectrum
        3 #prolate radius
        ${TEMP}/ImageSimulationMethodWithSpatialisationTest.tif
        ${TEMP}/LabelImageSimulationMethodWithSpatialisationTest.tif
        )



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSimulation_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(otbProspectReflTest ${Simulation_TESTS3}
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

#FIXME The tolerance is set to 0.01
#need to investigate differences between matlab baselines and otb implementation
ADD_TEST(otbProspectTransTest ${Simulation_TESTS3}
   --compare-ascii ${EPSILON_2} ${BASELINE_FILES}/prospect_trans.txt
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

ADD_TEST(otbSailReflHTest ${Simulation_TESTS3}
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
        
ADD_TEST(otbSailReflVTest ${Simulation_TESTS3}
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
        

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSimulation_TESTS4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(rsrSVMClassifier ${Simulation_TESTS4}
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
        
# ADD_TEST(atmosphericCorrectionsRSRSVMClassifier ${Simulation_TESTS4}
#         otbAtmosphericCorrectionsRSRSVMClassifier
#         ${SRCLASS0} 
#         ${SRCLASS1} 
#         ${SRCLASS2} 
#         ${SRCLASS3} 
#         ${SRCLASS4} 
#         ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
#         4 #nbBands
#         4 #day
#         12 #month
#         27.3 #elevation et azimuth solaire
#         152.7 #solar azimutal angle
#         2.5 #viewing zenithal angle
#         -77.0 #viewing azimutal angle
#         1013. #atmo pressure
#         2.48134 #water vapour amount
#         0.34400 #ozone amount
#         1 #aerosol model type 
#         0.199854 #aerosol optical
#         0.2 #percentage of validation files
#         )

ADD_TEST(ImageSimulationMethodKMeansClassif ${Simulation_TESTS4}
        otbImageSimulationMethodKMeansClassif
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb bands
        ${SPECTRUM_DB}/ #root path of spectrum
        3 #prolate radius
        ${TEMP}/ImageSimulationMethodKMeansClassif.tif
        ${TEMP}/KMeansClassifLabelImage.tif
        )

#warning the svm model estimation does not converge.
ADD_TEST(ImageSimulationMethodSVMClassif ${Simulation_TESTS4}
        otbImageSimulationMethodSVMClassif
        ${INPUTDATA}/Radiometry/SPOT5/HRG2/rep6S.dat
        4 #nb bands
        ${SPECTRUM_DB}/ #root path of spectrum
        3 #prolate radius
        ${TEMP}/ImageSimulationMethodSVMClassif.tif
        ${TEMP}/SVMClassifLabelImage.tif
        )

# atmospheric corrections tests 
SET(Simulation_SRCS1
otbSimulationTests1.cxx
otbSurfaceReflectanceToReflectanceFilterNew.cxx
)

# exploitation tests
SET(Simulation_SRCS2
otbSimulationTests2.cxx
otbSpectralResponseNew.cxx
otbSpectralResponse.cxx
otbSatelliteRSRNew.cxx
otbSatelliteRSR.cxx
otbReduceSpectralResponseNew.cxx
otbReduceSpectralResponse.cxx
otbReduceSpectralResponseClassifierRAndNIR.cxx
otbAtmosphericEffects.cxx
otbSpatialisationTest.cxx
otbLabelMapToSimulatedImageFilterTest.cxx
otbImageSimulationMethodWithVectorDataTest.cxx
otbImageSimulationMethodWithSpatialisationTest.cxx
)

# Prosail tests
SET(Simulation_SRCS3
otbSimulationTests3.cxx
otbProspectReflTest.cxx
otbProspectTransTest.cxx
otbSailReflHTest.cxx
otbSailReflVTest.cxx
)

# performance tests
SET(Simulation_SRCS4
otbSimulationTests4.cxx
otbReduceSpectralResponseSVMClassifier.cxx
otbAtmosphericCorrectionsRSRSVMClassifier.cxx
otbImageSimulationMethodKMeansClassif.cxx
otbImageSimulationMethodSVMClassif.cxx
)

#OTB_ADD_EXECUTABLE(otbSimulationTests1 "${Simulation_SRCS1}" "OTBSimulation;OTBIO;OTBTesting")
#OTB_ADD_EXECUTABLE(otbSimulationTests2 "${Simulation_SRCS2}" "OTBSimulation;OTBIO;OTBTesting")
#OTB_ADD_EXECUTABLE(otbSimulationTests3 "${Simulation_SRCS3}" "OTBSimulation;OTBIO;OTBTesting")
#OTB_ADD_EXECUTABLE(otbSimulationTests4 "${Simulation_SRCS4}" "OTBSimulation;OTBIO;OTBLearning;OTBTesting")

ADD_EXECUTABLE(otbSimulationTests1 ${Simulation_SRCS1})
TARGET_LINK_LIBRARIES(otbSimulationTests1 OTBIO OTBSimulation OTBTesting)
ADD_EXECUTABLE(otbSimulationTests2 ${Simulation_SRCS2})
TARGET_LINK_LIBRARIES(otbSimulationTests2 OTBIO OTBSimulation OTBTesting)
ADD_EXECUTABLE(otbSimulationTests3 ${Simulation_SRCS3})
TARGET_LINK_LIBRARIES(otbSimulationTests3 OTBIO OTBSimulation OTBTesting)
ADD_EXECUTABLE(otbSimulationTests4 ${Simulation_SRCS4})
TARGET_LINK_LIBRARIES(otbSimulationTests4 OTBIO OTBSimulation OTBLearning OTBTesting)



#ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
