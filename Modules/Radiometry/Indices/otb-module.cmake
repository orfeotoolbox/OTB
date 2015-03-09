set(DOCUMENTATION "With multispectral sensors, several indices can be computed
combining several spectral bands to show features that are not obvious using only
one band. This module contains filters that can compute classical indices, such
as NDVI (Normalized Difference Vegetation Index), NDWI (Normalized
DifferenceWater Index) and so on.")

otb_module(OTBIndices
  DEPENDS
    OTBCommon
    OTBFuzzy
    OTBITK
    OTBImageManipulation
    OTBMetadata
    OTBPath
    OTBVectorDataBase

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBObjectList
    OTBProjection
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
