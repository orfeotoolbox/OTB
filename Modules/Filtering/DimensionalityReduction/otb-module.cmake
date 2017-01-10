set(DOCUMENTATION "Dimension reduction is a statistical process, which
concentrates the amount of information in multivariate data into a fewer number of
variables (or dimensions). This module implements methods such as Principal Component
Analysis (PCA), Noise-Adjusted Principal Components Analysis (NAPCA), Maximum
Noise Fraction (MNF), Fast Independent Component Analysis (FICA), Maximum
Autocorrelation Factor (MAF), and so on.")

otb_module(OTBDimensionalityReduction
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBObjectList
    OTBStatistics

  TEST_DEPENDS
    OTBCommandLineParser
    OTBImageIO
    OTBTestKernel
    OTBWavelet

  DESCRIPTION
    "${DOCUMENTATION}"
)
