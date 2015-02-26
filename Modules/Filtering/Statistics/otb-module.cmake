set(DOCUMENTATION "This module contains classes related to statistical
calculation. One part of them is dedicated to the handling of ListSample (measurements
stored in a list type structure -as opposed to -). For instance, it is possible to
concatenate several sample lists into a single one, to generate a noised version
of the input sample list, and so on. The other part works on images and labelMaps
and allows the calculation of classical statistics (min max mean histogram).")

otb_module(OTBStatistics
  DEPENDS
    OTBCommon
    OTBITK
    OTBITKPendingPatches
    OTBImageBase
    OTBImageManipulation
    OTBObjectList
    OTBProjection
    OTBStreaming

  TEST_DEPENDS
    OTBCommandLineParser
    OTBImageIO
    OTBInterpolation
    OTBTestKernel
    OTBVectorDataBase
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
