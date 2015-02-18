project(OTBOpenJPEG)
set(OTBOpenJPEG_THIRD_PARTY 1)

set(OTBOpenJPEG_LIBRARIES otbopenjpeg)
set(OTBOpenJPEG_INCLUDE_DIRS
  ${OTBOpenJPEG_BINARY_DIR}/src
  )

otb_module_impl()
