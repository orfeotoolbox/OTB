set(DOCUMENTATION "Majority voting takes the more representative label of all
the pixels identified by the structuring element and then sets the center pixel to
this label value (see also DempsterShafer module).")

otb_module(OTBMajorityVoting
  DEPENDS
    OTBITK
    OTBOSSIMAdapters

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
