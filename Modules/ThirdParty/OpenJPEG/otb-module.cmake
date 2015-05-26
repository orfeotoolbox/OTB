set(DOCUMENTATION "This module imports OpenJPEG to the build system")

otb_module(OTBOpenJPEG
  DEPENDS
  
  TEST_DEPENDS
  
  DESCRIPTION
    "${DOCUMENTATION}"
)

otb_module_activation_option("Enable OpenJpeg dependent modules. Note that this may not be needed if your GDal has a JPEG2000 compatible driver (other than jasper)" OFF)
