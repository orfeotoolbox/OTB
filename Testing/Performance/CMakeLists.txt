# SET(PERFO_SVM_SRCS "otbReduceSpectralResponseSVMClassifier.cxx")

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

SET(SRCLASS0 ${OTB_SPECTRAL_RESPONSE}/JHU/becknic/manmade/txt)
SET(SRCLASS1 ${OTB_SPECTRAL_RESPONSE}/JHU/becknic/water/txt)
SET(SRCLASS2 ${OTB_SPECTRAL_RESPONSE}/JHU/becknic/soils/txt)
SET(SRCLASS3 ${OTB_SPECTRAL_RESPONSE}/JHU/becknic/vegetation/txt)
SET(SRCLASS4 ${OTB_SPECTRAL_RESPONSE}/JHU/becknic/rocks/sedimentary/powder/0_75/txt)
SET(SAT_RSR ${OTB_SAT_RSR}/SPOT5/HRG2)

SET(PERFO_TESTS ${CXX_TEST_PATH}/otbPerfoTests)

ADD_TEST(rsrSVMClassifier ${PERFO_TESTS}
        otbReduceSpectralResponseSVMClassifier
        ${SRCLASS0} 
        ${SRCLASS1} 
        ${SRCLASS2} 
        ${SRCLASS3} 
        ${SRCLASS4} 
        ${SAT_RSR}/rep6S.dat
        4 #nbBands
        0.1 #percentage of validation files
        )
        
ADD_TEST(atmosphericCorrectionsRSRSVMClassifier ${PERFO_TESTS}
        otbAtmosphericCorrectionsRSRSVMClassifier
        ${SRCLASS0} 
        ${SRCLASS1} 
        ${SRCLASS2} 
        ${SRCLASS3} 
        ${SRCLASS4} 
        ${SAT_RSR}/rep6S.dat
        4 #nbBands
        4 #day
        12 #month
        27.3 #elevation et azimuth solaire
        152.7 #solar azimutal angle
        2.5 #viewing zenithal angle
        -77.0 #viewing azimutal angle
        1013. #atmo pressure
        2.48134 #water vapour amount
        0.34400 #ozone amount
        1 #aerosol model type 
        0.199854 #aerosol optical
        0.2 #percentage of validation files
        )

SET(PERFO_SRCS
otbPerfoTests.cxx
otbReduceSpectralResponseSVMClassifier.cxx
otbAtmosphericCorrectionsRSRSVMClassifier.cxx
)


ADD_EXECUTABLE(otbPerfoTests ${PERFO_SRCS})
TARGET_LINK_LIBRARIES(otbPerfoTests OTBIO OTBCommon OTBRadiometry OTBLearning OTBTesting)


# INSTALL_TARGETS(/bin otbReduceSpectralResponseSVMClassifier)

