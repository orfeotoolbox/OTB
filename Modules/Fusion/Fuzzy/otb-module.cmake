set(DOCUMENTATION "A fuzzy variable is defined as a set of qualitative values
for a magnitude. These values are ordered, as for instance Low, Medium, High. This
module contains classes able to hold a vector where each component is the
membership value for each one of the qualitative values. it also contains classes able
to handle xml files where are stored several statistics.")

otb_module(OTBFuzzy 
  DEPENDS
    OTBCommon
    OTBTinyXML
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
