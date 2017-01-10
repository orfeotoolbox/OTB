set(DOCUMENTATION "TO BE DOCUMENTED")

otb_module(OTBCommon
ENABLE_SHARED
  DEPENDS
    OTBBoost
    OTBITK
    #Add dependency to OTBGDAL as GDAL module need to set OTB_USE_GDAL_20 before configuring otbConfigure.h
    OTBGDAL

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
