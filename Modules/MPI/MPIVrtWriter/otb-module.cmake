set(DOCUMENTATION "Provides a template function for MPI writing to a VRT file")

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
