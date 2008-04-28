
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)
SET(EPSILON_10e6 0.000001)
SET(MARKOV_TESTS1 ${CXX_TEST_PATH}/otbMarkovTests1)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMARKOV_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::MarkovClassificationFilter   ------------------------------

ADD_TEST(maTuMarkovClassificationFilterNew ${MARKOV_TESTS1}  
        otbMarkovClassificationFilterNew )
      
#ADD_TEST(raTvNDVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}  
#  --compare-image ${TOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
#                    ${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif
#        otbRAndNIRVegetationIndexImageFilter
#        NDVI
#	${INPUTDATA}/poupees_sub_c1.png
#	${INPUTDATA}/poupees_sub_c2.png
#	${TEMP}/raRAndNIRVegetationIndex_NDVI_poupees_subc1c2.tif 
#	)

# -------            otb::MRFEnergy       ------------------------------
ADD_TEST(maTuMRFEnergyNew ${MARKOV_TESTS1}  
        otbMRFEnergyNew )


# -------            otb::MRFEnergyGaussianClassification   ------------------------------
ADD_TEST(maTuMRFEnergyGaussianClassificationNew ${MARKOV_TESTS1}  
        otbMRFEnergyGaussianClassificationNew )


# -------            otb::MRFEnergyPotts   ------------------------------
ADD_TEST(maTuMRFEnergyPottsNew ${MARKOV_TESTS1}  
        otbMRFEnergyPottsNew )

# -------            otbMRFOptimizerNew   ------------------------------
ADD_TEST(maTuMRFOptimizerNew ${MARKOV_TESTS1}  
        otbMRFOptimizerNew )

# -------            otbMRFOptimizerMetropolisNew   ------------------------------
ADD_TEST(maTuMRFOptimizerMetropolisNew ${MARKOV_TESTS1}  
        otbMRFOptimizerMetropolisNew )

# -------            otbMRFSamplerNew     ------------------------------  
ADD_TEST(maTuMRFSamplerNew ${MARKOV_TESTS1}  
        otbMRFSamplerNew )



# -------            otbMRFSamplerRandomNew    ------------------------------
 ADD_TEST(maTuMRFSamplerRandomNew ${MARKOV_TESTS1}  
        otbMRFSamplerRandomNew )



# A enrichir
SET(Markov_SRCS1
otbMarkovClassificationFilterNew.cxx
otbMRFEnergyNew.cxx
otbMRFEnergyPottsNew.cxx
otbMRFEnergyGaussianClassificationNew.cxx
otbMRFOptimizerNew.cxx
otbMRFOptimizerMetropolisNew.cxx
otbMRFSamplerNew.cxx
otbMRFSamplerRandomNew.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMarkovTests1 otbMarkovTests1.cxx ${Markov_SRCS1})
TARGET_LINK_LIBRARIES(otbMarkovTests1 OTBMarkov OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )


