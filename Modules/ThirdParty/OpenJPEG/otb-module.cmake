set(DOCUMENTATION "This module provides OpenJPEG")

otb_module(OTBOpenJPEG
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable OpenJpeg dependent modules. Note that this may not be needed if your GDal has a JPEG2000 compatible driver (other than jasper)" ON)
