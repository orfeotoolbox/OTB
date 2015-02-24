set(DOCUMENTATION "Adapters for the Gdal Library ( for reading and writing  ).")

otb_module(OTBGdalAdapters
  DEPENDS
    OTBBoostAdapters
    OTBBoost
    OTBCommon
    OTBGDAL
    OTBITK

  DESCRIPTION
    "${DOCUMENTATION}"
)
