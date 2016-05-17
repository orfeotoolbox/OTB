set(DOCUMENTATION "TODO")

otb_module(OTBMPIVrtWriter
  DEPENDS
  OTBMPIConfig
    OTBPanSharpening
    OTBProjection
    OTBInterpolation
    OTBTestKernel
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
