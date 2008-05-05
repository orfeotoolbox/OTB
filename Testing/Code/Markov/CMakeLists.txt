
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
SET(MARKOV_TESTS2 ${CXX_TEST_PATH}/otbMarkovTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMARKOV_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::MarkovClassificationFilter   ------------------------------

ADD_TEST(maTuMarkovClassificationFilterNew ${MARKOV_TESTS1}  
        otbMarkovClassificationFilterNew )
      
# ADD_TEST(maTuMarkovClassificationFilter ${MARKOV_TESTS1}  
#   --compare-ascii ${TOL}  ${BASELINE_FILES}/maTuMarkovClassificationFilter.txt
#                   	  ${TEMP}/maTuMarkovClassificationFilter.txt
#         otbMarkovClassificationFilter
# 		${TEMP}/maTuMarkovClassificationFilter.txt 
# 	)

# -------            otb::MRFEnergy       ------------------------------
ADD_TEST(maTuMRFEnergyNew ${MARKOV_TESTS1}  
        otbMRFEnergyNew )

ADD_TEST(maTvMRFEnergy ${MARKOV_TESTS1} 
--compare-ascii ${TOL} ${BASELINE_FILES}/maTvMRFEnergy.txt
                         ${TEMP}/maTvMRFEnergy.txt
     otbMRFEnergy
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/maTvMRFEnergy.txt 
	)

# -------            otb::MRFEnergyEdgeFidelity       ------------------------------
ADD_TEST(maTuMRFEnergyEdgeFidelityNew ${MARKOV_TESTS1}  
        otbMRFEnergyEdgeFidelityNew )

ADD_TEST(maTvMRFEnergyEdgeFidelity ${MARKOV_TESTS1} 
--compare-ascii ${TOL} ${BASELINE_FILES}/maTvMRFEnergyEdgeFidelity.txt
                         ${TEMP}/maTvMRFEnergyEdgeFidelity.txt
     otbMRFEnergyEdgeFidelity
	${TEMP}/maTvMRFEnergyEdgeFidelity.txt 
	)


# -------            otb::MRFEnergyGaussianClassification   ------------------------------
ADD_TEST(maTuMRFEnergyGaussianClassificationNew ${MARKOV_TESTS1}  
        otbMRFEnergyGaussianClassificationNew )

ADD_TEST(maTvMRFEnergyGaussianClassification ${MARKOV_TESTS1} 
--compare-ascii ${TOL} ${BASELINE_FILES}/maTvMRFEnergyGaussianClassification.txt
                       ${TEMP}/maTvMRFEnergyGaussianClassification.txt
     otbMRFEnergyGaussianClassification
	${TEMP}/maTvMRFEnergyGaussianClassification.txt 
	)


# -------            otb::MRFEnergyPotts   ------------------------------
ADD_TEST(maTuMRFEnergyPottsNew ${MARKOV_TESTS1}  
        otbMRFEnergyPottsNew )

ADD_TEST(maTvMRFEnergyPotts ${MARKOV_TESTS1}  
  --compare-ascii ${TOL}   
                  ${BASELINE_FILES}/maTvMRFEnergyPotts.txt
                  ${TEMP}/maTvMRFEnergyPotts.txt
     otbMRFEnergyPotts
	${TEMP}/maTvMRFEnergyPotts.txt 
	)

# -------            otbMRFOptimizer   ------------------------------
ADD_TEST(maTuMRFOptimizerNew ${MARKOV_TESTS1}  
        otbMRFOptimizerNew )

# -------            otbMRFOptimizerICM   ------------------------------
ADD_TEST(maTuMRFOptimizerICMNew ${MARKOV_TESTS1}  
        otbMRFOptimizerICMNew )

ADD_TEST(maTuMRFOptimizerICM ${MARKOV_TESTS1}  
     otbMRFOptimizerICM
	${TEMP}/maTuMRFOptimizerICM.txt 
	)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMARKOV_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otbMRFOptimizerMetropolis   ------------------------------
ADD_TEST(maTuMRFOptimizerMetropolisNew ${MARKOV_TESTS2}  
        otbMRFOptimizerMetropolisNew )

ADD_TEST(maTvMRFOptimizerMetropolis ${MARKOV_TESTS2}
  --compare-ascii ${TOL}   
                  ${BASELINE_FILES}/maTvMRFOptimizerMetropolis.txt
                  ${TEMP}/maTvMRFOptimizerMetropolis.txt
     otbMRFOptimizerMetropolis
	${TEMP}/maTvMRFOptimizerMetropolis.txt 
	)


# -------            otbMRFSampler     ------------------------------  
ADD_TEST(maTuMRFSamplerNew ${MARKOV_TESTS2}  
        otbMRFSamplerNew )


# -------            otbMRFSamplerMAP     ------------------------------  
ADD_TEST(maTuMRFSamplerMAPNew ${MARKOV_TESTS2}  
        otbMRFSamplerMAPNew )
ADD_TEST(maTvMRFSamplerMAP ${MARKOV_TESTS2}
  --compare-ascii ${TOL}   
                  ${BASELINE_FILES}/maTvMRFSamplerMAP.txt
                  ${TEMP}/maTvMRFSamplerMAP.txt
     otbMRFSamplerMAP
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/maTvMRFSamplerMAP.txt 
	)


# -------            otbMRFSamplerRandom    ------------------------------
 ADD_TEST(maTuMRFSamplerRandomNew ${MARKOV_TESTS2}  
        otbMRFSamplerRandomNew )

ADD_TEST(maTvMRFSamplerRandom ${MARKOV_TESTS2}
  --compare-ascii ${TOL}   
                  ${BASELINE_FILES}/maTvMRFSamplerRandom.txt
                  ${TEMP}/maTvMRFSamplerRandom.txt
     otbMRFSamplerRandom
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/maTvMRFSamplerRandom.txt 
	)

# -------            otbMRFSamplerRandomMAP    ------------------------------
 ADD_TEST(maTuMRFSamplerRandomMAPNew ${MARKOV_TESTS2}  
        otbMRFSamplerRandomMAPNew )

ADD_TEST(maTvMRFSamplerRandomMAP ${MARKOV_TESTS2}
  --compare-ascii ${TOL}   
                  ${BASELINE_FILES}/maTvMRFSamplerRandomMAP.txt
                  ${TEMP}/maTvMRFSamplerRandomMAP.txt
     otbMRFSamplerRandomMAP
	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
	${TEMP}/maTvMRFSamplerRandomMAP.txt 
	)


# A enrichir
SET(Markov_SRCS1
otbMarkovClassificationFilterNew.cxx
otbMRFEnergyNew.cxx
otbMRFEnergy.cxx
otbMRFEnergyPottsNew.cxx
otbMRFEnergyPotts.cxx
otbMRFEnergyEdgeFidelityNew.cxx
otbMRFEnergyEdgeFidelity.cxx
otbMRFEnergyGaussianClassificationNew.cxx
otbMRFEnergyGaussianClassification.cxx
otbMRFOptimizerNew.cxx
otbMRFOptimizerICMNew.cxx
otbMRFOptimizerICM.cxx
)

SET(Markov_SRCS2
otbMRFOptimizerMetropolisNew.cxx
otbMRFOptimizerMetropolis.cxx
otbMRFSamplerNew.cxx
otbMRFSamplerMAPNew.cxx
otbMRFSamplerMAP.cxx
otbMRFSamplerRandomNew.cxx
otbMRFSamplerRandom.cxx
otbMRFSamplerRandomMAPNew.cxx
otbMRFSamplerRandomMAP.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMarkovTests1 otbMarkovTests1.cxx ${Markov_SRCS1})
TARGET_LINK_LIBRARIES(otbMarkovTests1 OTBMarkov OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(otbMarkovTests2 otbMarkovTests2.cxx ${Markov_SRCS2})
TARGET_LINK_LIBRARIES(otbMarkovTests2 OTBMarkov OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )


