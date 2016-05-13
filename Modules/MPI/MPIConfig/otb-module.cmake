set(DOCUMENTATION "TODO")

# OTB_module() defines the module dependencies in ExternalTemplate
# ExternalTemplate depends on OTBCommon and OTBApplicationEngine
# The testing module in ExternalTemplate depends on OTBTestKernel
# and OTBCommandLine

# define the dependencies of the include module and the tests
otb_module(OTBMPIConfig
  DEPENDS
    OTBMPI
    OTBCommon
  TEST_DEPENDS
    OTBMPITiffWriter
    OTBTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
)
