project(OTBConfigFile)
set(OTBConfigFile_THIRD_PARTY 1)

set(OTBConfigFile_LIBRARIES
  otbconfigfile
  )

set(OTBConfigFile_INCLUDE_DIRS
  ${OTBConfigFile_SOURCE_DIR}/src
  )

otb_module_impl()
