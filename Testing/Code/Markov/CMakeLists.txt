
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(MARKOV_TESTS1 ${CXX_TEST_PATH}/otbMarkovTests1)
set(MARKOV_TESTS2 ${CXX_TEST_PATH}/otbMarkovTests2)
set(MARKOV_TESTS3 ${CXX_TEST_PATH}/otbMarkovTests3)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMARKOV_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::MarkovRandomFieldFilter   ------------------------------

add_test(maTuMarkovRandomFieldFilterNew ${MARKOV_TESTS1}
        otbMarkovRandomFieldFilterNew )

add_test(maTvMarkovRandomFieldFilter ${MARKOV_TESTS1}
  --compare-image ${NOTOL}  ${BASELINE}/maTvMarkovRandomField.png
                            ${TEMP}/maTvMarkovRandomField.png
        otbMarkovRandomFieldFilter
            ${INPUTDATA}/QB_Suburb.png
            ${TEMP}/maTvMarkovRandomField.png
            1.0
            30
            1.0
            )


# -------            otb::MRFEnergyEdgeFidelity       ------------------------------
add_test(maTuMRFEnergyEdgeFidelityNew ${MARKOV_TESTS1}
        otbMRFEnergyEdgeFidelityNew )

add_test(maTvMRFEnergyEdgeFidelity ${MARKOV_TESTS1}
--compare-ascii ${NOTOL} ${BASELINE_FILES}/maTvMRFEnergyEdgeFidelity.txt
                         ${TEMP}/maTvMRFEnergyEdgeFidelity.txt
     otbMRFEnergyEdgeFidelity
        ${TEMP}/maTvMRFEnergyEdgeFidelity.txt
        )


# -------            otb::MRFEnergyGaussianClassification   ------------------------------
add_test(maTuMRFEnergyGaussianClassificationNew ${MARKOV_TESTS1}
        otbMRFEnergyGaussianClassificationNew )

add_test(maTvMRFEnergyGaussianClassification ${MARKOV_TESTS1}
--compare-ascii ${NOTOL} ${BASELINE_FILES}/maTvMRFEnergyGaussianClassification.txt
                       ${TEMP}/maTvMRFEnergyGaussianClassification.txt
     otbMRFEnergyGaussianClassification
        ${TEMP}/maTvMRFEnergyGaussianClassification.txt
        )


# -------            otb::MRFEnergyPotts   ------------------------------
add_test(maTuMRFEnergyPottsNew ${MARKOV_TESTS1}
        otbMRFEnergyPottsNew )

add_test(maTvMRFEnergyPotts ${MARKOV_TESTS1}
  --compare-ascii ${NOTOL}
                  ${BASELINE_FILES}/maTvMRFEnergyPotts.txt
                  ${TEMP}/maTvMRFEnergyPotts.txt
     otbMRFEnergyPotts
        ${TEMP}/maTvMRFEnergyPotts.txt
        )

# -------            otb::MRFEnergyGaussian   ------------------------------
add_test(maTuMRFEnergyGaussianNew ${MARKOV_TESTS1}
        otbMRFEnergyGaussianNew )

add_test(maTvMRFEnergyGaussian ${MARKOV_TESTS1}
     otbMRFEnergyGaussian
        )



# -------            otbMRFOptimizerICM   ------------------------------
add_test(maTuMRFOptimizerICMNew ${MARKOV_TESTS1}
        otbMRFOptimizerICMNew )

add_test(maTuMRFOptimizerICM ${MARKOV_TESTS1}
     otbMRFOptimizerICM
        ${TEMP}/maTuMRFOptimizerICM.txt
        )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMARKOV_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otbMRFOptimizerMetropolis   ------------------------------
add_test(maTuMRFOptimizerMetropolisNew ${MARKOV_TESTS2}
        otbMRFOptimizerMetropolisNew )

add_test(maTvMRFOptimizerMetropolis ${MARKOV_TESTS2}
  --compare-ascii ${NOTOL}
                  ${BASELINE_FILES}/maTvMRFOptimizerMetropolis.txt
                  ${TEMP}/maTvMRFOptimizerMetropolis.txt
     otbMRFOptimizerMetropolis
        ${TEMP}/maTvMRFOptimizerMetropolis.txt
        )


# -------            otbMRFSamplerMAP     ------------------------------
add_test(maTuMRFSamplerMAPNew ${MARKOV_TESTS2}
        otbMRFSamplerMAPNew )
add_test(maTvMRFSamplerMAP ${MARKOV_TESTS2}
  --compare-ascii ${NOTOL}
                  ${BASELINE_FILES}/maTvMRFSamplerMAP.txt
                  ${TEMP}/maTvMRFSamplerMAP.txt
     otbMRFSamplerMAP
        ${INPUTDATA}/poupees_sub_c1.png
        ${INPUTDATA}/poupees_sub_c2.png
        ${TEMP}/maTvMRFSamplerMAP.txt
        )


# -------            otbMRFSamplerRandom    ------------------------------
 add_test(maTuMRFSamplerRandomNew ${MARKOV_TESTS2}
        otbMRFSamplerRandomNew )

add_test(maTvMRFSamplerRandom ${MARKOV_TESTS2}
  --compare-ascii ${NOTOL}
                  ${BASELINE_FILES}/maTvMRFSamplerRandom.txt
                  ${TEMP}/maTvMRFSamplerRandom.txt
     otbMRFSamplerRandom
        ${INPUTDATA}/poupees_sub_c1.png
        ${INPUTDATA}/poupees_sub_c2.png
        ${TEMP}/maTvMRFSamplerRandom.txt
        )

# -------            otbMRFSamplerRandomMAP    ------------------------------
 add_test(maTuMRFSamplerRandomMAPNew ${MARKOV_TESTS2}
        otbMRFSamplerRandomMAPNew )

add_test(maTvMRFSamplerRandomMAP ${MARKOV_TESTS2}
  --compare-ascii ${NOTOL}
                  ${BASELINE_FILES}/maTvMRFSamplerRandomMAP.txt
                  ${TEMP}/maTvMRFSamplerRandomMAP.txt
     otbMRFSamplerRandomMAP
        ${INPUTDATA}/poupees_sub_c1.png
        ${INPUTDATA}/poupees_sub_c2.png
        ${TEMP}/maTvMRFSamplerRandomMAP.txt
        )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMARKOV_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::MRFEnergyFisherClassification   ------------------------------
add_test(maTuMRFEnergyFisherClassificationNew ${MARKOV_TESTS3}
        otbMRFEnergyFisherClassificationNew )

add_test(maTvMRFEnergyFisherClassification ${MARKOV_TESTS3}
    --compare-ascii ${NOTOL} ${BASELINE_FILES}/maTvMRFEnergyFisherClassification.txt
                            ${TEMP}/maTvMRFEnergyFisherClassification.txt
    otbMRFEnergyFisherClassification
        ${TEMP}/maTvMRFEnergyFisherClassification.txt
        )

# A enrichir
set(Markov_SRCS1
otbMarkovTests1.cxx
otbMarkovRandomFieldFilterNew.cxx
otbMarkovRandomFieldFilter.cxx
otbMRFEnergyPottsNew.cxx
otbMRFEnergyPotts.cxx
otbMRFEnergyGaussianNew.cxx
otbMRFEnergyGaussian.cxx
otbMRFEnergyEdgeFidelityNew.cxx
otbMRFEnergyEdgeFidelity.cxx
otbMRFEnergyGaussianClassificationNew.cxx
otbMRFEnergyGaussianClassification.cxx
otbMRFOptimizerICMNew.cxx
otbMRFOptimizerICM.cxx
)

set(Markov_SRCS2
otbMarkovTests2.cxx
otbMRFOptimizerMetropolisNew.cxx
otbMRFOptimizerMetropolis.cxx
otbMRFSamplerMAPNew.cxx
otbMRFSamplerMAP.cxx
otbMRFSamplerRandomNew.cxx
otbMRFSamplerRandom.cxx
otbMRFSamplerRandomMAPNew.cxx
otbMRFSamplerRandomMAP.cxx
)

set(Markov_SRCS3
otbMarkovTests3.cxx
otbMRFEnergyFisherClassification.cxx
)

OTB_ADD_EXECUTABLE(otbMarkovTests1 "${Markov_SRCS1}" "OTBMarkov;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbMarkovTests2 "${Markov_SRCS2}" "OTBMarkov;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbMarkovTests3 "${Markov_SRCS3}" "OTBMarkov;OTBIO;OTBTesting")

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
