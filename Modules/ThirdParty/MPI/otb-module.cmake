set(DOCUMENTATION "This module imports MPI to the build system")

otb_module(OTBMPI
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable MPI dependent modules" OFF)
