set(DOCUMENTATION "Dimension reduction is a statistical process, which
concentrates the amount of information in multivariate data into a fewer number of
variables (or dimensions). This module implements methods such as Principal Component
Analysis (PCA), Noise-Adjusted Principal Components Analysis (NAPCA), Maximum
Noise Fraction (MNF), Fast Independant Component Analysis (FICA), Maximum
Autocorrelation Factor (MAF), and so on.")

otb_module(OTBDimensionalityReduction
  DEPENDS
    OTBStatistics
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBCommon
    OTBObjectList

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBWavelet

  DESCRIPTION
    "${DOCUMENTATION}"
)
