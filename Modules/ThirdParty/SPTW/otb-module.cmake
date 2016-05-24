set(DOCUMENTATION "This module imports SPTW to the build system")

otb_module(OTBSPTW
  DEPENDS
    OTBMPI
    OTBGDAL

  TEST_DEPENDS

  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable SPTW dependent modules" ON)
