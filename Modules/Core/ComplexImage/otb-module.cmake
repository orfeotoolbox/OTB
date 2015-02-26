set(DOCUMENTATION "TODO")

otb_module(OTBComplexImage
  DEPENDS
    OTBColorMap
    OTBCommon

  TEST_DEPENDS
    OTBITK
    OTBImageBase
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
