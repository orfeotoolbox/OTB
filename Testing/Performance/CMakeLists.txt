# SET(PERFO_SVM_SRCS "otbReduceSpectralResponseSVMClassifier.cxx")

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

SET(SRCLASS0 ${OTB_SPECTRAL_RESPONSE}/Class0)
SET(SRCLASS1 ${OTB_SPECTRAL_RESPONSE}/Class1)
SET(SRCLASS2 ${OTB_SPECTRAL_RESPONSE}/Class2)
SET(SRCLASS3 ${OTB_SPECTRAL_RESPONSE}/Class3)
SET(SRCLASS4 ${OTB_SPECTRAL_RESPONSE}/Class4)
SET(SAT_RSR ${OTB_SAT_RSR}/SPOT5/HRG1)

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
        )

SET(PERFO_SRCS
otbPerfoTests.cxx
otbReduceSpectralResponseSVMClassifier.cxx
)


ADD_EXECUTABLE(otbPerfoTests ${PERFO_SRCS})
TARGET_LINK_LIBRARIES(otbPerfoTests OTBIO OTBCommon OTBRadiometry OTBLearning OTBTesting)

# INSTALL_TARGETS(/bin otbReduceSpectralResponseSVMClassifier)

